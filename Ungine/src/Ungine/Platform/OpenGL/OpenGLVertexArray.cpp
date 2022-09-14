#include "Upch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace U
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case U::ShaderDataType::Float:    return GL_FLOAT;
		case U::ShaderDataType::Float2:   return GL_FLOAT;
		case U::ShaderDataType::Float3:   return GL_FLOAT;
		case U::ShaderDataType::Float4:   return GL_FLOAT;
		case U::ShaderDataType::Mat3:     return GL_FLOAT;
		case U::ShaderDataType::Mat4:     return GL_FLOAT;
		case U::ShaderDataType::Int:      return GL_INT;
		case U::ShaderDataType::Int2:     return GL_INT;
		case U::ShaderDataType::Int3:     return GL_INT;
		case U::ShaderDataType::Int4:     return GL_INT;
		case U::ShaderDataType::Bool:     return GL_BOOL;
		}

		U_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;

	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		U_RENDER_S({
				glCreateVertexArrays(1,&self->m_RendererID);
			});
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		U_RENDER_S({
				glDeleteVertexArrays(1,&self->m_RendererID);
			});
	}

	void OpenGLVertexArray::Bind() const
	{
		U_RENDER_S({
				glBindVertexArray(self->m_RendererID);
			});
	}

	void OpenGLVertexArray::UnBind() const
	{
		U_RENDER_S({
				glBindVertexArray(0);
			});
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		U_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
		
		Bind();
		vertexBuffer->Bind();

		U_RENDER_S1(vertexBuffer, {
			const auto & layout = vertexBuffer->GetLayout();
			for (const auto& element : layout)
			{
				auto glBaseType = ShaderDataTypeToOpenGLBaseType(element.Type);
				glEnableVertexAttribArray(self->m_VertexBufferIndex);
				if (glBaseType == GL_INT)
				{
					glVertexAttribIPointer(self->m_VertexBufferIndex,
						element.GetComponentCount(),
						glBaseType,
						layout.GetStride(),
						(const void*)(intptr_t)element.Offset);
				}
				else
				{
					glVertexAttribPointer(self->m_VertexBufferIndex,
						element.GetComponentCount(),
						glBaseType,
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(intptr_t)element.Offset);
				}
				self->m_VertexBufferIndex++;
			}
			});
		m_VertexBuffers.push_back(vertexBuffer);

	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		Bind();
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;

	}

}