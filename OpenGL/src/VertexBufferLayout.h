// ReSharper disable CppExplicitSpecializationInNonNamespaceScope
#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct LayoutElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(const unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
			case GL_UNSIGNED_INT: return 4;
			default:
			{
				ASSERT(false);
				return 0;
			}
		}
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout()
		: m_Stride(0) {}

	template<typename T>
	void Push(unsigned int count)
	{
		ASSERT(false);
	}

	template<>
	void Push<float>(const unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * LayoutElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(const unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * LayoutElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(const unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * LayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	[[nodiscard]] std::vector<LayoutElement> GetElements() const { return m_Elements; }
	[[nodiscard]] unsigned int GetStride() const { return m_Stride; }
private:
	std::vector<LayoutElement> m_Elements;
	unsigned int m_Stride;
};
