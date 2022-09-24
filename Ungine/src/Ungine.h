#pragma once

#include "Ungine/Core/Application.h"
#include "Ungine/Core/Log.h"
#include "Ungine/Core/Input.h"
#include "Ungine/Core/Timestep.h"
#include "Ungine/Core/Timer.h"

#include "Ungine/Core/Events/Event.h"
#include "Ungine/Core/Events/ApplicationEvent.h"
#include "Ungine/Core/Events/KeyEvent.h"
#include "Ungine/Core/Events/MouseEvent.h"

#include "Ungine/Core/Math/AABB.h"
#include "Ungine/Core/Math/Ray.h"


#include "imgui/imgui.h"

// --- Ungine Render API ------------------------------
#include "Ungine/Renderer/Renderer.h"
#include "Ungine/Renderer/SceneRenderer.h"
#include "Ungine/Renderer/RenderPass.h"
#include "Ungine/Renderer/Framebuffer.h"
#include "Ungine/Renderer/Buffer.h"
#include "Ungine/Renderer/VertexArray.h"
#include "Ungine/Renderer/Texture.h"
#include "Ungine/Renderer/Shader.h"
#include "Ungine/Renderer/Mesh.h"
#include "Ungine/Renderer/Camera.h"
#include "Ungine/Renderer/Material.h"
// ---------------------------------------------------

//Scenes
#include "Ungine/Scene/Entity.h"
#include "Ungine/Scene/Scene.h"
#include "Ungine/Scene/Components.h"
#include "Ungine/Scene/SceneCamera.h"
#include "Ungine/Scene/SceneSerializer.h"
