#include "Upch.h"
#include "RenderPass.h"

#include "Renderer.h"

#include "Ungine/Platform/OpenGL/OpenGLRenderPass.h"

namespace U
{
	Ref<RenderPass> RenderPass::Create(const RenderPassSpecification& spec)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:    U_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPIType::OpenGL:  return std::make_shared<OpenGLRenderPass>(spec);
		}

		U_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;

	}
}