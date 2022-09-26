#include "Upch.h"
#include "IndexBuffer.h"

#include "Renderer.h"

#include "Ungine/Platform/OpenGL/OpenGLIndexBuffer.h"

namespace U
{
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:    return nullptr;
		case RendererAPIType::OpenGL:  return Ref<OpenGLIndexBuffer>::Create(size);
		}
		U_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}


	Ref<IndexBuffer> IndexBuffer::Create(void* data, uint32_t size)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:    return nullptr;
		case RendererAPIType::OpenGL:  return Ref<OpenGLIndexBuffer>::Create(data, size);
		}
		U_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}