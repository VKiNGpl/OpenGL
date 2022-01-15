#include "DynamicVertexBuffer.h"

#include "Renderer.h"

DynamicVertexBuffer::DynamicVertexBuffer(const unsigned int size)
{
	GL_CALL(glGenBuffers(1, &m_RendererID));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
}

DynamicVertexBuffer::~DynamicVertexBuffer()
{
	GL_CALL(glDeleteBuffers(1, &m_RendererID));
}

void DynamicVertexBuffer::Bind() const
{
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void DynamicVertexBuffer::Unbind()
{
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
