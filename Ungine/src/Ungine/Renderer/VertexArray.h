#pragma once
#include <memory>
#include "Ungine/Renderer/Buffer.h"

namespace U
{
	
	class VertexArray : public RefCounted
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		virtual RendererID GetRendererID() const = 0;

		static Ref<VertexArray> Create();
	};
}