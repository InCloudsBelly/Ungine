#include "Upch.h"

#include "Pipeline.h"

#include "Renderer.h"
#include "Ungine/Platform/OpenGL/OpenGLPipeline.h""

namespace U
{
	Ref<Pipeline> Pipeline::Create(const PipelineSpecification& spec)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:    return nullptr;
		case RendererAPIType::OpenGL:  return Ref<OpenGLPipeline>::Create(spec);
		}

		U_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}