#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray  // NOLINT(cppcoreguidelines-special-member-functions)
{
private:
	unsigned int m_RendererID{};
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;

	void Bind() const;
	static void Unbind();
};
