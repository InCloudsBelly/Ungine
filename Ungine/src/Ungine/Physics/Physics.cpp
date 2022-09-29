#include "Upch.h"
#include "Physics.h"
#include "PXPhysicsWrappers.h"

#include "PhysicsLayer.h"

namespace U
{
	static std::tuple<glm::vec3, glm::quat, glm::vec3> GetTransformDecomposition(const glm::mat4& transform)
	{
		glm::vec3 scale, translation, skew;
		glm::vec4 perspective;
		glm::quat orientation;
		glm::decompose(transform, scale, orientation, translation, skew, perspective);

		return { translation, orientation, scale };
	}


	static physx::PxScene* s_Scene;
	static std::vector<Entity> s_SimulatedEntities;
	static Entity* s_EntityStorageBuffer;
	static int s_EntityStorageBufferPosition;
	static float s_SimulationTime = 0.0f;

	void Physics::Init()
	{
		PXPhysicsWrappers::Initialize();
		PhysicsLayerManager::AddLayer("Default");
	}

	void Physics::Shutdown()
	{
		PXPhysicsWrappers::Shutdown();
	}

	void Physics::CreateScene(const SceneParams& params)
	{
		U_CORE_ASSERT(s_Scene == nullptr, "Scene already has a Physics Scene!");
		s_Scene = PXPhysicsWrappers::CreateScene(params);
	}

	void Physics::CreateActor(Entity e, int entityCount)
	{
		if (!e.HasComponent<RigidBodyComponent>())
		{
			U_CORE_WARN("Trying to create PhysX actor from a non-rigidbody actor!");
			return;
		}

		if (!e.HasComponent<PhysicsMaterialComponent>())
		{
			U_CORE_WARN("Trying to create PhysX actor without a PhysicsMaterialComponent!");
			return;
		}

		RigidBodyComponent& rigidbody = e.GetComponent<RigidBodyComponent>();

		if (s_EntityStorageBuffer == nullptr)
			s_EntityStorageBuffer = new Entity[entityCount];

		physx::PxRigidActor* actor = PXPhysicsWrappers::CreateActor(rigidbody, e.Transform());
		s_SimulatedEntities.push_back(e);
		Entity* entityStorage = &s_EntityStorageBuffer[s_EntityStorageBufferPosition++];
		*entityStorage = e;
		actor->userData = (void*)entityStorage;
		rigidbody.RuntimeActor = actor;

		physx::PxMaterial* material = PXPhysicsWrappers::CreateMaterial(e.GetComponent<PhysicsMaterialComponent>());

		const auto& transform = e.Transform();
		auto [translation, rotation, scale] = GetTransformDecomposition(transform);

		if (e.HasComponent<BoxColliderComponent>())
		{
			BoxColliderComponent& collider = e.GetComponent<BoxColliderComponent>();
			PXPhysicsWrappers::AddBoxCollider(*actor, *material, collider, scale);
		}

		if (e.HasComponent<SphereColliderComponent>())
		{
			SphereColliderComponent& collider = e.GetComponent<SphereColliderComponent>();
			PXPhysicsWrappers::AddSphereCollider(*actor, *material, collider, scale);
		}

		if (e.HasComponent<CapsuleColliderComponent>())
		{
			CapsuleColliderComponent& collider = e.GetComponent<CapsuleColliderComponent>();
			PXPhysicsWrappers::AddCapsuleCollider(*actor, *material, collider, scale);
		}

		if (e.HasComponent<MeshColliderComponent>())
		{
			MeshColliderComponent& collider = e.GetComponent<MeshColliderComponent>();
			PXPhysicsWrappers::AddMeshCollider(*actor, *material, collider, scale);
		}

		if (!PhysicsLayerManager::IsLayerValid(rigidbody.Layer))
			rigidbody.Layer = 0;

		PXPhysicsWrappers::SetCollisionFilters(*actor, rigidbody.Layer);

		s_Scene->addActor(*actor);
	}

	void Physics::Simulate(Timestep ts)
	{
		constexpr float stepSize = 1.0F / 50.0F;
		s_SimulationTime += ts.GetMilliseconds();

		if (s_SimulationTime < stepSize)
			return;

		s_SimulationTime -= stepSize;

		s_Scene->simulate(stepSize);
		s_Scene->fetchResults(true);

		for (Entity& e : s_SimulatedEntities)
		{
			auto& transform = e.Transform();
			auto [translation, rotation, scale] = GetTransformDecomposition(transform);
			RigidBodyComponent& rb = e.GetComponent<RigidBodyComponent>();
			physx::PxRigidActor* actor = static_cast<physx::PxRigidActor*>(rb.RuntimeActor);

			if (rb.BodyType == RigidBodyComponent::Type::Dynamic)
			{
				transform = FromPhysXTransform(actor->getGlobalPose()) * glm::scale(glm::mat4(1.0F), scale);
			}
			else if (rb.BodyType == RigidBodyComponent::Type::Static)
			{
				actor->setGlobalPose(ToPhysXTransform(transform));
			}
		}
	}

	void Physics::DestroyScene()
	{
		delete[] s_EntityStorageBuffer;
		s_EntityStorageBuffer = nullptr;
		s_EntityStorageBufferPosition = 0;
		s_SimulatedEntities.clear();
		s_Scene->release();
		s_Scene = nullptr;
	}

	void* Physics::GetPhysicsScene()
	{
		return s_Scene;
	}


	void Physics::ConnectVisualDebugger()
	{
		PXPhysicsWrappers::ConnectVisualDebugger();
	}

	void Physics::DisconnectVisualDebugger()
	{
		PXPhysicsWrappers::DisconnectVisualDebugger();
	}

}