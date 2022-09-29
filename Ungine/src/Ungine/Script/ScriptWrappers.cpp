#include "Upch.h"
#include "ScriptWrappers.h"

#include "Ungine/Core/Math/Noise.h"

#include "Ungine/Scene/Scene.h"
#include "Ungine/Scene/Components.h"
#include "Ungine/Physics/PhysicsUtil.h"
#include "Ungine/Physics/PXPhysicsWrappers.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <mono/jit/jit.h>

#include <box2d/box2d.h>

#include <PhysX/PxPhysicsAPI.h>

#include <imgui.h>

namespace U
{
	extern std::unordered_map<MonoType*, std::function<bool(Entity&)>> s_HasComponentFuncs;
	extern std::unordered_map<MonoType*, std::function<void(Entity&)>> s_CreateComponentFuncs;
}

namespace U
{
	namespace Script
	{
		enum class ComponentID
		{
			None = 0,
			Transform = 1,
			Mesh = 2,
			Script = 3,
			SpriteRenderer = 4
		};
	
		static std::tuple<glm::vec3, glm::quat, glm::vec3> GetTransformDecomposition(const glm::mat4& transform)
		{
			glm::vec3 scale, translation, skew;
			glm::vec4 perspective;
			glm::quat orientation;
			glm::decompose(transform, scale, orientation, translation, skew, perspective);

			return { translation, orientation, scale };
		}

		////////////////////////////////////////////////////////////////
		// Math ////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////

		float Ungine_Noise_PerlinNoise(float x, float y)
		{
			return Noise::PerlinNoise(x, y);
		}


		////////////////////////////////////////////////////////////////
		// Input ///////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////

		bool Ungine_Input_IsKeyPressed(KeyCode key)
		{
			return Input::IsKeyPressed(key);
		}

		bool Ungine_Input_IsMouseButtonPressed(MouseButton button)
		{
			return Input::IsMouseButtonPressed(button);
		}


		void Ungine_Input_GetMousePosition(glm::vec2* outPosition)
		{
			auto [x, y] = Input::GetMousePosition();
			*outPosition = { x, y };
		}

		void Ungine_Input_SetCursorMode(CursorMode mode)
		{
			Input::SetCursorMode(mode);
		}

		CursorMode Ungine_Input_GetCursorMode()
		{
			return Input::GetCursorMode();
		}

		bool Ungine_Physics_Raycast(glm::vec3* origin, glm::vec3* direction, float maxDistance, RaycastHit* hit)
		{
			return PXPhysicsWrappers::Raycast(*origin, *direction, maxDistance, hit);
		}


		////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////
		// Entity //////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////

		void Ungine_Entity_GetTransform(uint64_t entityID, glm::mat4* outTransform)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			U_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();

			U_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
			
			Entity entity = entityMap.at(entityID);
			auto& transformComponent = entity.GetComponent<TransformComponent>();
			memcpy(outTransform, glm::value_ptr(transformComponent.Transform), sizeof(glm::mat4));
		}

		void Ungine_Entity_SetTransform(uint64_t entityID, glm::mat4* inTransform)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			U_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			U_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			auto& transformComponent = entity.GetComponent<TransformComponent>();
			memcpy(glm::value_ptr(transformComponent.Transform), inTransform, sizeof(glm::mat4));
		}

		void Ungine_Entity_CreateComponent(uint64_t entityID, void* type)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			U_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			U_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);

			s_CreateComponentFuncs[monoType](entity);
		}

		bool Ungine_Entity_HasComponent(uint64_t entityID, void* type)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			U_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			U_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);

			bool result = s_HasComponentFuncs[monoType](entity);
			return result;
		}

		uint64_t Ungine_Entity_FindEntityByTag(MonoString* tag)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			U_CORE_ASSERT(scene, "No active scene!");

			Entity entity = scene->FindEntityByTag(mono_string_to_utf8(tag));
			if (entity)
				return entity.GetComponent<IDComponent>().ID;

			return 0;
		}

		void Ungine_TransformComponent_GetRelativeDirection(uint64_t entityID, glm::vec3* outDirection, glm::vec3* inAbsoluteDirection)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			U_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			U_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			auto& transformComponent = entity.GetComponent<TransformComponent>();

			auto [position, rotation, scale] = GetTransformDecomposition(transformComponent.Transform);
			*outDirection = glm::rotate(rotation, *inAbsoluteDirection);
		}

		void Ungine_TransformComponent_GetRotation(uint64_t entityID, glm::vec3* outRotation)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			U_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			U_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			auto& transformComponent = entity.GetComponent<TransformComponent>();
			auto [position, rotationQuat, scale] = GetTransformDecomposition(transformComponent.Transform);
			*outRotation = glm::degrees(glm::eulerAngles(rotationQuat));
		}

		void Ungine_TransformComponent_SetRotation(uint64_t entityID, glm::vec3* inRotation)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			U_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			U_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			glm::mat4& transform = entity.Transform();

			auto [translation, rotationQuat, scale] = GetTransformDecomposition(transform);
			transform = glm::translate(glm::mat4(1.0F), translation) *
				glm::toMat4(glm::quat(glm::radians(*inRotation))) *
				glm::scale(glm::mat4(1.0F), scale);
		}


		void* Ungine_MeshComponent_GetMesh(uint64_t entityID)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			U_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			U_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			auto& meshComponent = entity.GetComponent<MeshComponent>();
			return new Ref<Mesh>(meshComponent.Mesh);
		}

		void Ungine_MeshComponent_SetMesh(uint64_t entityID, Ref<Mesh>* inMesh)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			U_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			U_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);

			auto& meshComponent = entity.GetComponent<MeshComponent>();
			meshComponent.Mesh = inMesh ? *inMesh : nullptr;
		}


		void Ungine_RigidBody2DComponent_ApplyLinearImpulse(uint64_t entityID, glm::vec2* impulse, glm::vec2* offset, bool wake)
		{
			Ref<Scene> scene= ScriptEngine::GetCurrentSceneContext();
			U_CORE_ASSERT(scene, "NO active scene!");
			const auto& entityMap = scene->GetEntityMap();
			U_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			U_CORE_ASSERT(entity.HasComponent<RigidBody2DComponent>());
			auto& component = entity.GetComponent<RigidBody2DComponent>();
			b2Body* body = (b2Body*)component.RuntimeBody;
			body->ApplyLinearImpulse(*(const b2Vec2*)impulse, body->GetWorldCenter() + *(const b2Vec2*)offset, wake);
		}
	
		void Ungine_RigidBody2DComponent_GetLinearVelocity(uint64_t entityID, glm::vec2* outVelocity)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			U_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			U_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			U_CORE_ASSERT(entity.HasComponent<RigidBody2DComponent>());
			auto& component = entity.GetComponent<RigidBody2DComponent>();
			b2Body* body = (b2Body*)component.RuntimeBody;
			const auto& velocity = body->GetLinearVelocity();
			U_CORE_ASSERT(outVelocity);
			*outVelocity = { velocity.x, velocity.y };
		}

		void Ungine_RigidBody2DComponent_SetLinearVelocity(uint64_t entityID, glm::vec2* velocity)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			U_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			U_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			U_CORE_ASSERT(entity.HasComponent<RigidBody2DComponent>());
			auto& component = entity.GetComponent<RigidBody2DComponent>();
			b2Body* body = (b2Body*)component.RuntimeBody;
			U_CORE_ASSERT(velocity);
			body->SetLinearVelocity({ velocity->x, velocity->y });
		}

		void Ungine_RigidBodyComponent_AddForce(uint64_t entityID, glm::vec3* force, ForceMode forceMode)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			U_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			U_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			U_CORE_ASSERT(entity.HasComponent<RigidBodyComponent>());
			
			auto& component = entity.GetComponent<RigidBodyComponent>();
			if (component.IsKinematic)
			{
				U_CORE_WARN("Cannot add a force to a kinematic actor! EntityID({0})", entityID);
				return;
			}

			physx::PxRigidActor* actor = (physx::PxRigidActor*)component.RuntimeActor;
			physx::PxRigidDynamic* dynamicActor = actor->is<physx::PxRigidDynamic>();
			U_CORE_ASSERT(dynamicActor);

			U_CORE_ASSERT(force);
			dynamicActor->addForce({ force->x, force->y, force->z }, (physx::PxForceMode::Enum)forceMode);

		}

		void Ungine_RigidBodyComponent_AddTorque(uint64_t entityID, glm::vec3* torque, ForceMode forceMode)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			U_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			U_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			U_CORE_ASSERT(entity.HasComponent<RigidBodyComponent>());
			auto& component = entity.GetComponent<RigidBodyComponent>();

			if (component.IsKinematic)
			{
				U_CORE_WARN("Cannot add torque to a kinematic actor! EntityID({0})", entityID);
				return;
			}


			physx::PxRigidActor* actor = (physx::PxRigidActor*)component.RuntimeActor;
			physx::PxRigidDynamic* dynamicActor = actor->is<physx::PxRigidDynamic>();

			U_CORE_ASSERT(dynamicActor);

			U_CORE_ASSERT(torque);
			dynamicActor->addTorque({ torque->x, torque->y, torque->z }, (physx::PxForceMode::Enum)forceMode);
		}

		void Ungine_RigidBodyComponent_GetLinearVelocity(uint64_t entityID, glm::vec3* outVelocity)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			U_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			U_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			U_CORE_ASSERT(entity.HasComponent<RigidBodyComponent>());
			auto& component = entity.GetComponent<RigidBodyComponent>();

			physx::PxRigidActor* actor = (physx::PxRigidActor*)component.RuntimeActor;
			physx::PxRigidDynamic* dynamicActor = actor->is<physx::PxRigidDynamic>();

			U_CORE_ASSERT(dynamicActor);

			U_CORE_ASSERT(outVelocity);
			physx::PxVec3 velocity = dynamicActor->getLinearVelocity();
			*outVelocity = { velocity.x, velocity.y, velocity.z };
		}

		void Ungine_RigidBodyComponent_SetLinearVelocity(uint64_t entityID, glm::vec3* velocity)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			U_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			U_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			U_CORE_ASSERT(entity.HasComponent<RigidBodyComponent>());
			auto& component = entity.GetComponent<RigidBodyComponent>();

			physx::PxRigidActor* actor = (physx::PxRigidActor*)component.RuntimeActor;
			physx::PxRigidDynamic* dynamicActor = actor->is<physx::PxRigidDynamic>();

			U_CORE_ASSERT(dynamicActor);

			U_CORE_ASSERT(velocity);
			dynamicActor->setLinearVelocity({ velocity->x, velocity->y, velocity->z });
		}

		void Ungine_RigidBodyComponent_Rotate(uint64_t entityID, glm::vec3* rotation)
		{
			Ref<Scene> scene = ScriptEngine::GetCurrentSceneContext();
			U_CORE_ASSERT(scene, "No active scene!");
			const auto& entityMap = scene->GetEntityMap();
			U_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

			Entity entity = entityMap.at(entityID);
			U_CORE_ASSERT(entity.HasComponent<RigidBodyComponent>());
			auto& component = entity.GetComponent<RigidBodyComponent>();

			physx::PxRigidActor* actor = (physx::PxRigidActor*)component.RuntimeActor;
			physx::PxRigidDynamic* dynamicActor = actor->is<physx::PxRigidDynamic>();
			U_CORE_ASSERT(dynamicActor);

			physx::PxTransform transform = dynamicActor->getGlobalPose();
			transform.q *= (physx::PxQuat(glm::radians(rotation->x), { 1.0F, 0.0F, 0.0F })
				* physx::PxQuat(glm::radians(rotation->y), { 0.0F, 1.0F, 0.0F })
				* physx::PxQuat(glm::radians(rotation->z), { 0.0F, 0.0F, 1.0F }));
			dynamicActor->setGlobalPose(transform);
		}



		Ref<Mesh>* Ungine_Mesh_Constructor(MonoString* filepath)
		{
			return new Ref<Mesh>(new Mesh(mono_string_to_utf8(filepath)));
		}

		void Ungine_Mesh_Destructor(Ref<Mesh>* _this)
		{
			Ref<Mesh>* instance = (Ref<Mesh>*)_this;
			delete _this;
		}

		Ref<Material>* Ungine_Mesh_GetMaterial(Ref<Mesh>* inMesh)
		{
			Ref<Mesh>& mesh = *(Ref<Mesh>*)inMesh;
			return new Ref<Material>(mesh->GetMaterial());
		}

		Ref<MaterialInstance>* Ungine_Mesh_GetMaterialByIndex(Ref<Mesh>* inMesh, int index)
		{
			Ref<Mesh>& mesh = *(Ref<Mesh>*)inMesh;
			const auto& materials = mesh->GetMaterials();

			U_CORE_ASSERT(index < materials.size());
			return new Ref<MaterialInstance>(materials[index]);
		}

		int Ungine_Mesh_GetMaterialCount(Ref<Mesh>* inMesh)
		{
			Ref<Mesh>& mesh = *(Ref<Mesh>*)inMesh;
			const auto& materials = mesh->GetMaterials();
			return materials.size();
		}

		void* Ungine_Texture2D_Constructor(uint32_t width, uint32_t height)
		{
			auto result = Texture2D::Create(TextureFormat::RGBA, width, height);
			return new Ref<Texture2D>(result);
		}

		void Ungine_Texture2D_Destructor(Ref<Texture2D>* _this)
		{
			delete _this;
		}

		void Ungine_Texture2D_SetData(Ref<Texture2D>* _this, MonoArray* inData, int32_t count)
		{
			Ref<Texture2D>& instance = *_this;

			uint32_t dataSize = count * sizeof(glm::vec4) / 4;

			instance->Lock();
			Buffer buffer = instance->GetWriteableBuffer();
			U_CORE_ASSERT(dataSize <= buffer.Size);
			// Convert RGBA32F color to RGBA8
			uint8_t* pixels = (uint8_t*)buffer.Data;
			uint32_t index = 0;
			for (int i = 0; i < instance->GetWidth() * instance->GetHeight(); i++)
			{
				glm::vec4& value = mono_array_get(inData, glm::vec4, i);
				*pixels++ = (uint32_t)(value.x * 255.0f);
				*pixels++ = (uint32_t)(value.y * 255.0f);
				*pixels++ = (uint32_t)(value.z * 255.0f);
				*pixels++ = (uint32_t)(value.w * 255.0f);
			}

			instance->Unlock();
		}

		void Ungine_Material_Destructor(Ref<Material>* _this)
		{
			delete _this;
		}

		void Ungine_Material_SetFloat(Ref<Material>* _this, MonoString* uniform, float value)
		{
			Ref<Material>& instance = *(Ref<Material>*)_this;
			instance->Set(mono_string_to_utf8(uniform), value);
		}

		void Ungine_Material_SetTexture(Ref<Material>* _this, MonoString* uniform, Ref<Texture2D>* texture)
		{
			Ref<Material>& instance = *(Ref<Material>*)_this;
			instance->Set(mono_string_to_utf8(uniform), *texture);
		}

		void Ungine_MaterialInstance_Destructor(Ref<MaterialInstance>* _this)
		{
			delete _this;
		}

		void Ungine_MaterialInstance_SetFloat(Ref<MaterialInstance>* _this, MonoString* uniform, float value)
		{
			Ref<MaterialInstance>& instance = *(Ref<MaterialInstance>*)_this;
			instance->Set(mono_string_to_utf8(uniform), value);
		}

		void Ungine_MaterialInstance_SetVector3(Ref<MaterialInstance>* _this, MonoString* uniform, glm::vec3* value)
		{
			Ref<MaterialInstance>& instance = *(Ref<MaterialInstance>*)_this;
			instance->Set(mono_string_to_utf8(uniform), *value);
		}

		void Ungine_MaterialInstance_SetVector4(Ref<MaterialInstance>* _this, MonoString* uniform, glm::vec4* value)
		{
			Ref<MaterialInstance>& instance = *(Ref<MaterialInstance>*)_this;
			instance->Set(mono_string_to_utf8(uniform), *value);
		}

		void Ungine_MaterialInstance_SetTexture(Ref<MaterialInstance>* _this, MonoString* uniform, Ref<Texture2D>* texture)
		{
			Ref<MaterialInstance>& instance = *(Ref<MaterialInstance>*)_this;
			instance->Set(mono_string_to_utf8(uniform), *texture);
		}

		void* Ungine_MeshFactory_CreatePlane(float width, float height)
		{
			// TODO: Implement properly with MeshFactory class!
			return new Ref<Mesh>(new Mesh("assets/models/Plane1m.obj"));
		}

		////////////////////////////////////////////////////////////////


	}
}