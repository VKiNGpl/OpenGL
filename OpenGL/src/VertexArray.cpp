#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GL_CALL(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GL_CALL(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned* offset = nullptr;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& [type, count, normalized] = elements[i];

		GL_CALL(glEnableVertexAttribArray(i));
		GL_CALL(glVertexAttribPointer(i, count, type, normalized, layout.GetStride(), offset));

		offset = offset + static_cast<unsigned long long>(count) * LayoutElement::GetSizeOfType(type);
	}
}

void VertexArray::Bind() const
{
	GL_CALL(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind()
{
	GL_CALL(glBindVertexArray(0));
}
