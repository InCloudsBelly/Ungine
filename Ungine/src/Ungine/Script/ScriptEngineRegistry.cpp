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
	}


	void ScriptEngineRegistry::RegisterAll()
	{
		InitComponentTypes();

		mono_add_internal_call("Ungine.Noise::PerlinNoise_Native", U::Script::Ungine_Noise_PerlinNoise);

		mono_add_internal_call("Ungine.Entity::GetTransform_Native", U::Script::Ungine_Entity_GetTransform);
		mono_add_internal_call("Ungine.Entity::SetTransform_Native", U::Script::Ungine_Entity_SetTransform);
		mono_add_internal_call("Ungine.Entity::CreateComponent_Native", U::Script::Ungine_Entity_CreateComponent);
		mono_add_internal_call("Ungine.Entity::HasComponent_Native", U::Script::Ungine_Entity_HasComponent);

		mono_add_internal_call("Ungine.MeshComponent::GetMesh_Native", U::Script::Ungine_MeshComponent_GetMesh);
		mono_add_internal_call("Ungine.MeshComponent::SetMesh_Native", U::Script::Ungine_MeshComponent_SetMesh);

		mono_add_internal_call("Ungine.Input::IsKeyPressed_Native", U::Script::Ungine_Input_IsKeyPressed);

		mono_add_internal_call("Ungine.Texture2D::Constructor_Native", U::Script::Ungine_Texture2D_Constructor);
		mono_add_internal_call("Ungine.Texture2D::Destructor_Native", U::Script::Ungine_Texture2D_Destructor);
		mono_add_internal_call("Ungine.Texture2D::SetData_Native", U::Script::Ungine_Texture2D_SetData);

		mono_add_internal_call("Ungine.Material::Destructor_Native", U::Script::Ungine_Material_Destructor);
		mono_add_internal_call("Ungine.Material::SetFloat_Native", U::Script::Ungine_Material_SetFloat);
		mono_add_internal_call("Ungine.Material::SetTexture_Native", U::Script::Ungine_Material_SetTexture);

		mono_add_internal_call("Ungine.MaterialInstance::Destructor_Native", U::Script::Ungine_MaterialInstance_Destructor);
		mono_add_internal_call("Ungine.MaterialInstance::SetFloat_Native", U::Script::Ungine_MaterialInstance_SetFloat);
		mono_add_internal_call("Ungine.MaterialInstance::SetVector3_Native", U::Script::Ungine_MaterialInstance_SetVector3);
		mono_add_internal_call("Ungine.MaterialInstance::SetTexture_Native", U::Script::Ungine_MaterialInstance_SetTexture);

		mono_add_internal_call("Ungine.Mesh::Constructor_Native", U::Script::Ungine_Mesh_Constructor);
		mono_add_internal_call("Ungine.Mesh::Destructor_Native", U::Script::Ungine_Mesh_Destructor);
		mono_add_internal_call("Ungine.Mesh::GetMaterial_Native", U::Script::Ungine_Mesh_GetMaterial);
		mono_add_internal_call("Ungine.Mesh::GetMaterialByIndex_Native", U::Script::Ungine_Mesh_GetMaterialByIndex);
		mono_add_internal_call("Ungine.Mesh::GetMaterialCount_Native", U::Script::Ungine_Mesh_GetMaterialCount);

		mono_add_internal_call("Ungine.MeshFactory::CreatePlane_Native", U::Script::Ungine_MeshFactory_CreatePlane);

		// static bool IsKeyPressed(KeyCode key) { return s_Instance->IsKeyPressedImpl(key); }
		// 
		// static bool IsMouseButtonPressed(MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		// static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		// static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		// static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
	}



}