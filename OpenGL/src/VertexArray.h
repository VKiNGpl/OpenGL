#pragma once

#include "VertexBuffer.h"
#include "DynamicVertexBuffer.h"

class VertexBufferLayout;

class VertexArray  // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;
	void AddDynamicBuffer(const DynamicVertexBuffer& dvb, const VertexBufferLayout& layout) const;

	void Bind() const;
	static void Unbind();
private:
	unsigned int m_RendererID{};
};
