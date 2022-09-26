#pragma once

#include "Ungine/Core/Ref.h"

#include "Ungine/Renderer/VertexBuffer.h"
#include "Ungine/Renderer/Shader.h"

namespace U
{
	struct PipelineSpecification
	{
		Ref<Shader> Shader;
		VertexBufferLayout Layout;
	};

	class Pipeline : public RefCounted
	{
	public:
		virtual ~Pipeline() = default;
		virtual const PipelineSpecification& GetSpecification() const = 0;

		virtual void Invalidate() = 0;

		// TEMP: remove this when render command buffers are a thing
		virtual void Bind() = 0;

		static Ref<Pipeline> Create(const PipelineSpecification& spec);
	};
}