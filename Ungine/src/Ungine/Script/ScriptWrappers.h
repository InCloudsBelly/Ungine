#pragma once

#include "Ungine/Script/ScriptEngine.h"
#include "Ungine/Core/KeyCodes.h"

#include <glm/glm.hpp>

extern "C"
{
	typedef struct _MonoString MonoString;
	typedef struct _MonoArray MonoArray;
}

namespace U {
	namespace Script {
		// Math
		float Ungine_Noise_PerlinNoise(float x, float y);

		// Input
		bool Ungine_Input_IsKeyPressed(KeyCode key);

		// Entity
		void Ungine_Entity_GetTransform(uint64_t entityID, glm::mat4* outTransform);
		void Ungine_Entity_SetTransform(uint64_t entityID, glm::mat4* inTransform);
		void Ungine_Entity_CreateComponent(uint64_t entityID, void* type);
		bool Ungine_Entity_HasComponent(uint64_t entityID, void* type);


		void* Ungine_MeshComponent_GetMesh(uint64_t entityID);
		void Ungine_MeshComponent_SetMesh(uint64_t entityID, Ref<Mesh>* inMesh);


		// Renderer
		// Texture2D
		void* Ungine_Texture2D_Constructor(uint32_t width, uint32_t height);
		void Ungine_Texture2D_Destructor(Ref<Texture2D>* _this);
		void Ungine_Texture2D_SetData(Ref<Texture2D>* _this, MonoArray* inData, int32_t count);

		// Material
		void Ungine_Material_Destructor(Ref<Material>* _this);
		void Ungine_Material_SetFloat(Ref<Material>* _this, MonoString* uniform, float value);
		void Ungine_Material_SetTexture(Ref<Material>* _this, MonoString* uniform, Ref<Texture2D>* texture);

		void Ungine_MaterialInstance_Destructor(Ref<MaterialInstance>* _this);
		void Ungine_MaterialInstance_SetFloat(Ref<MaterialInstance>* _this, MonoString* uniform, float value);
		void Ungine_MaterialInstance_SetVector3(Ref<MaterialInstance>* _this, MonoString* uniform, glm::vec3* value);
		void Ungine_MaterialInstance_SetTexture(Ref<MaterialInstance>* _this, MonoString* uniform, Ref<Texture2D>* texture);

		// Mesh
		Ref<Mesh>* Ungine_Mesh_Constructor(MonoString* filepath);
		void Ungine_Mesh_Destructor(Ref<Mesh>* _this);
		Ref<Material>* Ungine_Mesh_GetMaterial(Ref<Mesh>* inMesh);
		Ref<MaterialInstance>* Ungine_Mesh_GetMaterialByIndex(Ref<Mesh>* inMesh, int index);
		int Ungine_Mesh_GetMaterialCount(Ref<Mesh>* inMesh);

		void* Ungine_MeshFactory_CreatePlane(float width, float height);
	}
}