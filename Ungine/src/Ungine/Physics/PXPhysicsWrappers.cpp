#include "Upch.h"
#include "PXPhysicsWrappers.h"
#include "Physics.h"

#ifdef  U_DEBUG
#define PHYSX_DEBUGGER 0
#endif 

namespace U
{
	static physx::PxDefaultErrorCallback s_ErrorCallback;
	static physx::PxDefaultAllocator s_Allocator;
	static physx::PxFoundation* s_Foundation;
	static physx::PxPhysics* s_Physics;
	static physx::PxPvd* s_VisualDebugger = nullptr;
	static physx::PxCooking* s_CookingFactory;

	static physx::PxSimulationFilterShader s_FilterShader = physx::PxDefaultSimulationFilterShader;

	static ContactListener s_ContactListener;

	physx::PxScene* PXPhysicsWrappers::CreateScene(const SceneParams& sceneParams)
	{
		physx::PxSceneDesc sceneDesc(s_Physics->getTolerancesScale());

		sceneDesc.gravity = ToPhysXVector(sceneParams.Gravity);
		sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
		sceneDesc.filterShader = UngineFilterShader;
		sceneDesc.simulationEventCallback = &s_ContactListener;

		U_CORE_ASSERT(sceneDesc.isValid());
		return s_Physics->createScene(sceneDesc);
	}

	physx::PxRigidActor* PXPhysicsWrappers::CreateActor(const RigidBodyComponent& rigidbody, const glm::mat4& transform)
	{
		physx::PxRigidActor* actor = nullptr;

		if (rigidbody.BodyType == RigidBodyComponent::Type::Static)
		{
			actor = s_Physics->createRigidStatic(ToPhysXTransform(transform));
		}
		else if (rigidbody.BodyType == RigidBodyComponent::Type::Dynamic)
		{
			physx::PxRigidDynamic* dynamicActor = s_Physics->createRigidDynamic(ToPhysXTransform(transform));

			dynamicActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, rigidbody.IsKinematic);

			dynamicActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, rigidbody.LockPositionX);
			dynamicActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, rigidbody.LockPositionY);
			dynamicActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, rigidbody.LockPositionZ);
			dynamicActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidbody.LockRotationX);
			dynamicActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, rigidbody.LockRotationY);
			dynamicActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, rigidbody.LockRotationZ);

			physx::PxRigidBodyExt::updateMassAndInertia(*dynamicActor, rigidbody.Mass);
			actor = dynamicActor;
		}

		return actor;
	}

	void PXPhysicsWrappers::SetCollisionFilters(const physx::PxRigidActor& actor, uint32_t actorGroup, uint32_t filters)
	{
		physx::PxFilterData filterData;
		filterData.word0 = actorGroup;
		filterData.word1 = filters;

		const physx::PxU32 numShapes = actor.getNbShapes();

		physx::PxShape** shapes = (physx::PxShape**)s_Allocator.allocate(sizeof(physx::PxShape*) * numShapes, "", "", 0);
		actor.getShapes(shapes, numShapes);

		for (physx::PxU32 i = 0; i < numShapes; i++)
			shapes[i]->setSimulationFilterData(filterData);

		s_Allocator.deallocate(shapes);
	}

	void PXPhysicsWrappers::AddBoxCollider(physx::PxRigidActor& actor, const physx::PxMaterial& material, const BoxColliderComponent& collider, const glm::vec3& size)
	{
		glm::vec3 colliderSize = collider.Size;

		if (size.x != 0.0F) colliderSize.x *= size.x;
		if (size.y != 0.0F) colliderSize.y *= size.y;
		if (size.z != 0.0F) colliderSize.z *= size.z;

		physx::PxBoxGeometry boxGeometry = physx::PxBoxGeometry(colliderSize.x / 2.0F, colliderSize.y / 2.0F, colliderSize.z / 2.0F);

		physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(actor, boxGeometry, material);
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !collider.IsTrigger);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, collider.IsTrigger);
		shape->setLocalPose(ToPhysXTransform(glm::translate(glm::mat4(1.0F), collider.Offset)));
	}

	void PXPhysicsWrappers::AddSphereCollider(physx::PxRigidActor& actor, const physx::PxMaterial& material, const SphereColliderComponent& collider, const glm::vec3& size) {
		float colliderRadius = collider.Radius;

		if (size.x != 0.0F) colliderRadius *= size.x;

		physx::PxSphereGeometry sphereGeometry = physx::PxSphereGeometry(colliderRadius);

		physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(actor, sphereGeometry, material);
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !collider.IsTrigger);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, collider.IsTrigger);

	}

	void PXPhysicsWrappers::AddCapsuleCollider(physx::PxRigidActor& actor, const physx::PxMaterial& material, const CapsuleColliderComponent& collider, const glm::vec3& size)
	{
		float colliderRadius = collider.Radius;
		float colliderHeight = collider.Height;

		if (size.x != 0.0F)
			colliderRadius *= size.x;

		if (size.y != 0.0F)
			colliderHeight *= size.y;

		physx::PxCapsuleGeometry capsuleGeometry = physx::PxCapsuleGeometry(colliderRadius, colliderHeight / 2.0F);

		physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(actor, capsuleGeometry, material);
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !collider.IsTrigger);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, collider.IsTrigger);
		shape->setLocalPose(physx::PxTransform(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1))));
	}

	void PXPhysicsWrappers::AddMeshCollider(physx::PxRigidActor& actor, const physx::PxMaterial& material, MeshColliderComponent& collider, const glm::vec3& size)
	{
		// TODO: Possibly take a look at https://github.com/kmammou/v-hacd for computing convex meshes from triangle meshes...
		physx::PxConvexMeshGeometry triangleGeometry = physx::PxConvexMeshGeometry(CreateConvexMesh(collider));
		triangleGeometry.meshFlags = physx::PxConvexMeshGeometryFlag::eTIGHT_BOUNDS;
		physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(actor, triangleGeometry, material);
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !collider.IsTrigger);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, collider.IsTrigger);
	}

	physx::PxConvexMesh* PXPhysicsWrappers::CreateConvexMesh(MeshColliderComponent& collider)
	{
		const auto& vertices = collider.CollisionMesh->GetStaticVertices();
		const auto& indices = collider.CollisionMesh->GetIndices();

		physx::PxConvexMeshDesc convexDesc;
		convexDesc.points.count = vertices.size();
		convexDesc.points.stride = sizeof(Vertex);
		convexDesc.points.data = vertices.data();
		convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

		physx::PxDefaultMemoryOutputStream buf;
		physx::PxConvexMeshCookingResult::Enum result;
		if (!s_CookingFactory->cookConvexMesh(convexDesc, buf, &result))
			U_CORE_ASSERT(false);

		physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		physx::PxConvexMesh* mesh = s_Physics->createConvexMesh(input);
	
		if (!collider.ProcessedMesh)
		{
			// Based On: https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/ThirdParty/PhysX3/NvCloth/samples/SampleBase/renderer/ConvexRenderMesh.cpp
			const uint32_t nbPolygons = mesh->getNbPolygons();
			const physx::PxVec3* convexVertices = mesh->getVertices();
			const physx::PxU8* convexIndices = mesh->getIndexBuffer();

			uint32_t nbVertices = 0;
			uint32_t nbFaces = 0;

			for (uint32_t i = 0; i < nbPolygons; i++)
			{
				physx::PxHullPolygon polygon;
				mesh->getPolygonData(i, polygon);
				nbVertices += polygon.mNbVerts;
				nbFaces += (polygon.mNbVerts - 2) * 3;
			}

			std::vector<Vertex> collisionVertices;
			std::vector<Index> collisionIndices;

			collisionVertices.resize(nbVertices);
			collisionIndices.resize(nbFaces / 3);

			uint32_t vertCounter = 0;
			uint32_t indexCounter = 0;
			for (uint32_t i = 0; i < nbPolygons; i++)
			{
				physx::PxHullPolygon polygon;
				mesh->getPolygonData(i, polygon);

				uint32_t vI0 = vertCounter;
				for (uint32_t vI = 0; vI < polygon.mNbVerts; vI++)
				{
					collisionVertices[vertCounter].Position = FromPhysXVector(convexVertices[convexIndices[polygon.mIndexBase + vI]]);
					vertCounter++;
				}

				for (uint32_t vI = 1; vI < uint32_t(polygon.mNbVerts) - 1; vI++)
				{
					collisionIndices[indexCounter].V1 = uint32_t(vI0);
					collisionIndices[indexCounter].V2 = uint32_t(vI0 + vI + 1);
					collisionIndices[indexCounter].V3 = uint32_t(vI0 + vI);
					indexCounter++;
				}
			}

			collider.ProcessedMesh = Ref<Mesh>::Create(collisionVertices, collisionIndices);
		}

		return mesh;
	}

	physx::PxMaterial* PXPhysicsWrappers::CreateMaterial(const PhysicsMaterialComponent& material)
	{
		return s_Physics->createMaterial(material.StaticFriction, material.DynamicFriction, material.Bounciness);
	}

	void PXPhysicsWrappers::Initialize()
	{
		U_CORE_ASSERT(!s_Foundation, "PXPhysicsWrappers::Initializer shouldn't be called more than once!");

		s_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, s_Allocator, s_ErrorCallback);
		U_CORE_ASSERT(s_Foundation, "PxCreateFoundation Failed!");

#if PHYSX_DEBUGGER
		s_VisualDebugger = PxCreatePvd(*s_Foundation);
		ConnectVisualDebugger();
#endif

		s_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *s_Foundation, physx::PxTolerancesScale(), true, s_VisualDebugger);
		U_CORE_ASSERT(s_Physics, "PxCreatePhysics Failed!");

		s_CookingFactory = PxCreateCooking(PX_PHYSICS_VERSION, *s_Foundation, s_Physics->getTolerancesScale());
		U_CORE_ASSERT(s_CookingFactory, "PxCreatePhysics Failed!");
	}

	void PXPhysicsWrappers::Shutdown()
	{
		s_Physics->release();
		s_Foundation->release();
	}

	void PXPhysicsWrappers::ConnectVisualDebugger()
	{
#if PHYSX_DEBUGGER
		if (s_VisualDebugger->isConnected(false))
			s_VisualDebugger->disconnect();

		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
		s_VisualDebugger->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
#endif
	}

	void PXPhysicsWrappers::DisconnectVisualDebugger()
	{
#if PHYSX_DEBUGGER
		if (s_VisualDebugger->isConnected(false))
			s_VisualDebugger->disconnect();
#endif
	}



}
