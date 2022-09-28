#include "Upch.h"
#include "Physics.h"
#include "PXPhysicsWrappers.h"

#include "Ungine/Scene/Scene.h"

namespace U
{
	static physx::PxScene* s_Scene;
	static std::vector<Entity> s_SimulatedEntities;
	static Entity* s_EntityStorageBuffer;
	static int s_EntityStorageBufferPosition;

	static std::tuple<glm::vec3, glm::quat, glm::vec3> GetTransformDecomposition(const glm::mat4& transform)
	{
		glm::vec3 scale, translation, skew;
		glm::vec4 perspective;
		glm::quat orientation;
		glm::decompose(transform, scale, orientation, translation, skew, perspective);

		return { translation, orientation, scale };
	}

	void Physics::Init()
	{
		PXPhysicsWrappers::Initialize();
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

		// Create Actor Body
		physx::PxRigidActor* actor = PXPhysicsWrappers::CreateActor(rigidbody, e.Transform());
		s_SimulatedEntities.push_back(e);
		Entity* entityStorage = &s_EntityStorageBuffer[s_EntityStorageBufferPosition++];
		*entityStorage = e;
		actor->userData = (void*)entityStorage;
		rigidbody.RuntimeActor = actor;

		// Physics Material
		physx::PxMaterial* material = PXPhysicsWrappers::CreateMaterial(e.GetComponent<PhysicsMaterialComponent>());

		auto [translation, rotationQuat, scale] = GetTransformDecomposition(e.Transform());

		// Add all colliders
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

		// Set collision filters
		if (rigidbody.BodyType == RigidBodyComponent::Type::Static)
		{
			PXPhysicsWrappers::SetCollisionFilters(*actor, (uint32_t)FilterGroup::Static, (uint32_t)FilterGroup::All);
		}
		else if (rigidbody.BodyType == RigidBodyComponent::Type::Dynamic)
		{
			PXPhysicsWrappers::SetCollisionFilters(*actor, (uint32_t)FilterGroup::Dynamic, (uint32_t)FilterGroup::All);
		}

		s_Scene->addActor(*actor);
	}

	void Physics::Simulate()
	{
		constexpr float stepSize = 0.016666660f;
		s_Scene->simulate(stepSize);
		s_Scene->fetchResults(true);

		for (Entity& e : s_SimulatedEntities)
		{
			auto& transform = e.Transform();
			// TODO: Come up with a better solution for scale
			auto [p, r, scale] = GetTransformDecomposition(transform);
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

	void Physics::ConnectVisualDebugger()
	{
		PXPhysicsWrappers::ConnectVisualDebugger();
	}

	void Physics::DisconnectVisualDebugger()
	{
		PXPhysicsWrappers::DisconnectVisualDebugger();
	}

}