#include"Upch.h"
#include "ScriptEngineRegistry.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Ungine/Scene/Entity.h"
#include "ScriptWrappers.h"
#include <iostream>


namespace U
{
	std::unordered_map<MonoType*, std::function<bool(Entity&)>> s_HasComponentFuncs;
	std::unordered_map<MonoType*, std::function<void(Entity&)>> s_CreateComponentFuncs;

	extern MonoImage* s_CoreAssemblyImage;

#define Component_RegisterType(Type) \
	{\
		MonoType* type = mono_reflection_type_from_name("U." #Type, s_CoreAssemblyImage);\
		if (type) {\
			uint32_t id = mono_type_get_type(type);\
			s_HasComponentFuncs[type] = [](Entity& entity) { return entity.HasComponent<Type>(); };\
			s_CreateComponentFuncs[type] = [](Entity& entity) { entity.AddComponent<Type>(); };\
		} else {\
			U_CORE_ERROR("No C# component class found for " #Type "!");\
		}\
	}

	static void InitComponentTypes()
	{
		Component_RegisterType(TagComponent);
		Component_RegisterType(TransformComponent);
		Component_RegisterType(MeshComponent);
		Component_RegisterType(ScriptComponent);
		Component_RegisterType(CameraComponent);
		Component_RegisterType(SpriteRendererComponent);
		Component_RegisterType(RigidBody2DComponent);
		Component_RegisterType(BoxCollider2DComponent);
		Component_RegisterType(RigidBodyComponent);
		Component_RegisterType(BoxColliderComponent);
		Component_RegisterType(SphereColliderComponent);
	}

	void ScriptEngineRegistry::RegisterAll()
	{
		InitComponentTypes();

		mono_add_internal_call("U.Noise::PerlinNoise_Native", U::Script::Ungine_Noise_PerlinNoise);

		mono_add_internal_call("U.Entity::GetTransform_Native", U::Script::Ungine_Entity_GetTransform);
		mono_add_internal_call("U.Entity::SetTransform_Native", U::Script::Ungine_Entity_SetTransform);
		mono_add_internal_call("U.Entity::CreateComponent_Native", U::Script::Ungine_Entity_CreateComponent);
		mono_add_internal_call("U.Entity::HasComponent_Native", U::Script::Ungine_Entity_HasComponent);
		mono_add_internal_call("U.Entity::FindEntityByTag_Native", U::Script::Ungine_Entity_FindEntityByTag);

		mono_add_internal_call("U.TransformComponent::GetTransform_Native", U::Script::Ungine_Entity_GetTransform);
		mono_add_internal_call("U.TransformComponent::SetTransform_Native", U::Script::Ungine_Entity_SetTransform);
		mono_add_internal_call("U.TransformComponent::GetRelativeDirection_Native", U::Script::Ungine_TransformComponent_GetRelativeDirection);

		mono_add_internal_call("U.MeshComponent::GetMesh_Native", U::Script::Ungine_MeshComponent_GetMesh);
		mono_add_internal_call("U.MeshComponent::SetMesh_Native", U::Script::Ungine_MeshComponent_SetMesh);

		mono_add_internal_call("U.RigidBody2DComponent::ApplyLinearImpulse_Native", U::Script::Ungine_RigidBody2DComponent_ApplyLinearImpulse);
		mono_add_internal_call("U.RigidBody2DComponent::GetLinearVelocity_Native", U::Script::Ungine_RigidBody2DComponent_GetLinearVelocity);
		mono_add_internal_call("U.RigidBody2DComponent::SetLinearVelocity_Native", U::Script::Ungine_RigidBody2DComponent_SetLinearVelocity);

		mono_add_internal_call("U.RigidBodyComponent::AddForce_Native", U::Script::Ungine_RigidBodyComponent_AddForce);
		mono_add_internal_call("U.RigidBodyComponent::AddTorque_Native", U::Script::Ungine_RigidBodyComponent_AddTorque);
		mono_add_internal_call("U.RigidBodyComponent::GetLinearVelocity_Native", U::Script::Ungine_RigidBodyComponent_GetLinearVelocity);
		mono_add_internal_call("U.RigidBodyComponent::SetLinearVelocity_Native", U::Script::Ungine_RigidBodyComponent_SetLinearVelocity);

		mono_add_internal_call("U.Input::IsKeyPressed_Native", U::Script::Ungine_Input_IsKeyPressed);

		mono_add_internal_call("U.Texture2D::Constructor_Native", U::Script::Ungine_Texture2D_Constructor);
		mono_add_internal_call("U.Texture2D::Destructor_Native", U::Script::Ungine_Texture2D_Destructor);
		mono_add_internal_call("U.Texture2D::SetData_Native", U::Script::Ungine_Texture2D_SetData);

		mono_add_internal_call("U.Material::Destructor_Native", U::Script::Ungine_Material_Destructor);
		mono_add_internal_call("U.Material::SetFloat_Native", U::Script::Ungine_Material_SetFloat);
		mono_add_internal_call("U.Material::SetTexture_Native", U::Script::Ungine_Material_SetTexture);

		mono_add_internal_call("U.MaterialInstance::Destructor_Native", U::Script::Ungine_MaterialInstance_Destructor);
		mono_add_internal_call("U.MaterialInstance::SetFloat_Native", U::Script::Ungine_MaterialInstance_SetFloat);
		mono_add_internal_call("U.MaterialInstance::SetVector3_Native", U::Script::Ungine_MaterialInstance_SetVector3);
		mono_add_internal_call("U.MaterialInstance::SetVector4_Native", U::Script::Ungine_MaterialInstance_SetVector4);
		mono_add_internal_call("U.MaterialInstance::SetTexture_Native", U::Script::Ungine_MaterialInstance_SetTexture);

		mono_add_internal_call("U.Mesh::Constructor_Native", U::Script::Ungine_Mesh_Constructor);
		mono_add_internal_call("U.Mesh::Destructor_Native", U::Script::Ungine_Mesh_Destructor);
		mono_add_internal_call("U.Mesh::GetMaterial_Native", U::Script::Ungine_Mesh_GetMaterial);
		mono_add_internal_call("U.Mesh::GetMaterialByIndex_Native", U::Script::Ungine_Mesh_GetMaterialByIndex);
		mono_add_internal_call("U.Mesh::GetMaterialCount_Native", U::Script::Ungine_Mesh_GetMaterialCount);

		mono_add_internal_call("U.MeshFactory::CreatePlane_Native", U::Script::Ungine_MeshFactory_CreatePlane);

		// static bool IsKeyPressed(KeyCode key) { return s_Instance->IsKeyPressedImpl(key); }
		// 
		// static bool IsMouseButtonPressed(MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		// static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		// static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		// static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
	}




}