#include "Upch.h"

#include "VertexArray.h"

#include "Renderer.h"
#include "Ungine/Platform/OpenGL/OpenGLVertexArray.h""

namespace U
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:    U_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPIType::OpenGL:  return std::make_shared<OpenGLVertexArray>();
		}

		U_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}