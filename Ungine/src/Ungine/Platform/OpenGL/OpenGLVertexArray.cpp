#include "Upch.h"
#include "OpenGLVertexArray.h"

#include "Ungine/Renderer/Renderer.h"

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
		Renderer::Submit([this]() {
				glCreateVertexArrays(1,&m_RendererID);
			});
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		GLuint rendererID = m_RendererID;
		Renderer::Submit([rendererID]() {
			glDeleteVertexArrays(1, &rendererID);
			});
	}

	void OpenGLVertexArray::Bind() const
	{
		Ref<const OpenGLVertexArray> instance = this;
		Renderer::Submit([instance]() {
			glBindVertexArray(instance->m_RendererID);
			});

	}

	void OpenGLVertexArray::UnBind() const
	{
		Ref<const OpenGLVertexArray> instance = this;
		Renderer::Submit([this]() {
				glBindVertexArray(0);
			});
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) 
	{
		U_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
		
		Bind();
		vertexBuffer->Bind();
		Ref<OpenGLVertexArray> instance = this;
		Renderer::Submit([instance, vertexBuffer]() mutable 
			{
				const auto & layout = vertexBuffer->GetLayout();
				for (const auto& element : layout)
				{
					auto glBaseType = ShaderDataTypeToOpenGLBaseType(element.Type);
					glEnableVertexAttribArray(instance->m_VertexBufferIndex);
					if (glBaseType == GL_INT)
					{
						glVertexAttribIPointer(instance->m_VertexBufferIndex,
							element.GetComponentCount(),
							glBaseType,
							layout.GetStride(),
							(const void*)(intptr_t)element.Offset);
					}
					else
					{
						glVertexAttribPointer(instance->m_VertexBufferIndex,
							element.GetComponentCount(),
							glBaseType,
							element.Normalized ? GL_TRUE : GL_FALSE,
							layout.GetStride(),
							(const void*)(intptr_t)element.Offset);
					}
					instance->m_VertexBufferIndex++;
				}
			});
		m_VertexBuffers.push_back(vertexBuffer);

	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		Bind();
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;

	}

}