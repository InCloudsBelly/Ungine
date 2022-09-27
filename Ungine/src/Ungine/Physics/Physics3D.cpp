#include "Upch.h"
#include "Physics3D.h"
#include "Ungine/Script/ScriptEngine.h"

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <PhysX/PxPhysicsAPI.h>

#ifdef HZ_DEBUG
#define PHYSX_DEBUGGER 1
#endif


namespace U
{
	class PhysXContactListener :public physx::PxSimulationEventCallback
	{
	public:
		virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override
		{
			PX_UNUSED(constraints);
			PX_UNUSED(count);
		}

		virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override
		{
			PX_UNUSED(actors);
			PX_UNUSED(count);
		}

		virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override
		{
			PX_UNUSED(actors);
			PX_UNUSED(count);
		}


		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override
		{
			Entity& a = *(Entity*)pairHeader.actors[0]->userData;
			Entity& b = *(Entity*)pairHeader.actors[1]->userData;

			if (pairs->flags == physx::PxContactPairFlag::eACTOR_PAIR_HAS_FIRST_TOUCH)
			{
				if (a.HasComponent<ScriptComponent>() && ScriptEngine::ModuleExists(a.GetComponent<ScriptComponent>().ModuleName))
					ScriptEngine::OnCollisionBegin(a);

				if (b.HasComponent<ScriptComponent>() && ScriptEngine::ModuleExists(b.GetComponent<ScriptComponent>().ModuleName))
					ScriptEngine::OnCollisionBegin(b);
			}
			else if (pairs->flags == physx::PxContactPairFlag::eACTOR_PAIR_LOST_TOUCH)
			{
				if (a.HasComponent<ScriptComponent>() && ScriptEngine::ModuleExists(a.GetComponent<ScriptComponent>().ModuleName))
					ScriptEngine::OnCollisionEnd(a);

				if (b.HasComponent<ScriptComponent>() && ScriptEngine::ModuleExists(b.GetComponent<ScriptComponent>().ModuleName))
					ScriptEngine::OnCollisionEnd(b);
			}
		}

		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override
		{
			PX_UNUSED(pairs);
			PX_UNUSED(count);
		}

		virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override
		{
			PX_UNUSED(bodyBuffer);
			PX_UNUSED(poseBuffer);
			PX_UNUSED(count);
		}
	};

	static PhysXContactListener s_PhysXContactListener;

	static physx::PxSimulationFilterShader s_DefaultFilterShader = physx::PxDefaultSimulationFilterShader;

	static std::tuple<glm::vec3, glm::quat, glm::vec3> GetTransformDecomposition(const glm::mat4& transform)
	{
		glm::vec3 scale, translation, skew;
		glm::vec4 perspective;
		glm::quat orientation;
		glm::decompose(transform, scale, orientation, translation, skew, perspective);
		return { translation, orientation, scale };
	}

	static physx::PxFilterFlags UngineFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, 
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, 
		physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
	{
		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
			return physx::PxFilterFlag::eDEFAULT;
		}

		pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

		if ((filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1))
		{
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
		}

		return physx::PxFilterFlag::eDEFAULT;
	}



	void Physics3D::Init()
	{
		U_CORE_ASSERT(!s_PXFoundation, "PhysXManager::Init shouldn't be called more than once!");

		s_PXFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, s_PXAllocator, s_PXErrorCallback);
		U_CORE_ASSERT(s_PXFoundation, "PxCreateFoundation Failed!");

#if PHYSX_DEBUGGER
		s_PXPvd = PxCreatePvd(*s_PXFoundation);
		ConnectToPhysXDebugger();
#endif
	
		s_PXPhysicsFactory = PxCreatePhysics(PX_PHYSICS_VERSION, *s_PXFoundation, physx::PxTolerancesScale(), true, s_PXPvd);
		U_CORE_ASSERT(s_PXPhysicsFactory, "PxCreatePhysics Failed!");

		s_PXCookingFactory = PxCreateCooking(PX_PHYSICS_VERSION, *s_PXFoundation, s_PXPhysicsFactory->getTolerancesScale());
		U_CORE_ASSERT(s_PXCookingFactory, "PxCreatePhysics Failed!");

	}

	void Physics3D::Shutdown()
	{
		DisconnectFromPhysXDebugger();

		s_PXPhysicsFactory->release();
		s_PXFoundation->release();
	}

	physx::PxSceneDesc Physics3D::CreateSceneDesc()
	{
		physx::PxSceneDesc sceneDesc(s_PXPhysicsFactory->getTolerancesScale());
		
		physx::PxDefaultCpuDispatcher* mCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
		if (!mCpuDispatcher)
			U_CORE_ASSERT(false);

		sceneDesc.cpuDispatcher = mCpuDispatcher;
		sceneDesc.filterShader = UngineFilterShader;
		sceneDesc.simulationEventCallback = &s_PhysXContactListener;

		return sceneDesc;
	}

	physx::PxScene* Physics3D::CreateScene(const physx::PxSceneDesc& sceneDesc)
	{
		return s_PXPhysicsFactory->createScene(sceneDesc);
	}


	physx::PxRigidActor* Physics3D::CreateAndAddActor(physx::PxScene* scene, const RigidBodyComponent& rigidbody, const glm::mat4& transform)
	{
		physx::PxRigidActor* actor = nullptr;

		if (rigidbody.BodyType == RigidBodyComponent::Type::Static)
		{
			actor = s_PXPhysicsFactory->createRigidStatic(CreatePose(transform));
		}
		else if (rigidbody.BodyType == RigidBodyComponent::Type::Dynamic)
		{
			physx::PxRigidDynamic* dynamicActor = s_PXPhysicsFactory->createRigidDynamic(CreatePose(transform));
			
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

		scene->addActor(*actor);

		return actor;
	}

	physx::PxMaterial* Physics3D::CreateMaterial(float staticFriction, float dynamicFriction, float restitution)
	{
		return s_PXPhysicsFactory->createMaterial(staticFriction, dynamicFriction, restitution);
	}


	physx::PxConvexMesh* Physics3D::CreateMeshCollider(const Ref<Mesh>& mesh)
	{
		// TODO: Possibly take a look at https://github.com/kmammou/v-hacd for computing convex meshes from triangle meshes...

		const auto& vertices = mesh->GetStaticVertices();
		const auto& indices = mesh->GetIndices();

		physx::PxConvexMeshDesc convexDesc;
		convexDesc.points.count = vertices.size();
		convexDesc.points.stride = sizeof(Vertex);
		convexDesc.points.data = vertices.data();
		convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

		physx::PxDefaultMemoryOutputStream buf;
		physx::PxConvexMeshCookingResult::Enum result;
		if (!s_PXCookingFactory->cookConvexMesh(convexDesc, buf, &result))
			return nullptr;

		physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		return s_PXPhysicsFactory->createConvexMesh(input);
	}

	/*physx::PxTriangleMesh* Physics3D::CreateMeshCollider(const Ref<Mesh>& mesh)
	{
		const auto& vertices = mesh->GetStaticVertices();
		const auto& indices = mesh->GetIndices();

		physx::PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = vertices.size();
		meshDesc.points.stride = sizeof(Vertex);
		meshDesc.points.data = vertices.data();

		meshDesc.triangles.count = indices.size();
		meshDesc.triangles.stride = sizeof(Index);
		meshDesc.triangles.data = indices.data();

		physx::PxDefaultMemoryOutputStream writeBuffer;
		physx::PxTriangleMeshCookingResult::Enum result;
		bool status = s_PXCookingFactory->cookTriangleMesh(meshDesc, writeBuffer, &result);
		if (!status)
			return nullptr;

		physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
		return s_PXPhysicsFactory->createTriangleMesh(readBuffer);
	}*/


	physx::PxTransform Physics3D::CreatePose(const glm::mat4& transform)
	{
		auto [translation, rotationQuat, scale] = GetTransformDecomposition(transform);
		glm::vec3 rotation = glm::eulerAngles(rotationQuat);

		physx::PxTransform physxTransform(physx::PxVec3(translation.x, translation.y, translation.z));
		physxTransform.rotate(physx::PxVec3(rotation.x, rotation.y, rotation.z));
		return physxTransform;
	}

	void Physics3D::SetCollisionFilters(physx::PxRigidActor* actor, uint32_t filterGroup, uint32_t filterMask)
	{
		physx::PxFilterData filterData;
		filterData.word0 = filterGroup; // word0 = own ID
		filterData.word1 = filterMask;
		// contact callback;
		const physx::PxU32 numShapes = actor->getNbShapes();
		physx::PxShape** shapes = (physx::PxShape**)s_PXAllocator.allocate(sizeof(physx::PxShape*) * numShapes, "", "", 0);
		actor->getShapes(shapes, numShapes);
		for (physx::PxU32 i = 0; i < numShapes; i++)
		{
			physx::PxShape* shape = shapes[i];
			shape->setSimulationFilterData(filterData);
		}
		s_PXAllocator.deallocate(shapes);
	}

	void Physics3D::ConnectToPhysXDebugger()
	{
#if PHYSX_DEBUGGER
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
		s_PXPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
#endif
	}

	void Physics3D::DisconnectFromPhysXDebugger()
	{
#if PHYSX_DEBUGGER
		if (s_PXPvd->isConnected(false))
			s_PXPvd->disconnect();
#endif
	}


	physx::PxDefaultErrorCallback Physics3D::s_PXErrorCallback;
	physx::PxDefaultAllocator Physics3D::s_PXAllocator;
	physx::PxFoundation* Physics3D::s_PXFoundation;
	physx::PxPhysics* Physics3D::s_PXPhysicsFactory;
	physx::PxPvd* Physics3D::s_PXPvd;
	physx::PxCooking* Physics3D::s_PXCookingFactory;
}