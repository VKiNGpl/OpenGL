#pragma once

/**
 * \brief OpenGL Vertex Buffer Class
 */
class VertexBuffer  // NOLINT(cppcoreguidelines-special-member-functions)
{
private:
	/**
	 * \brief ID of this vertex buffer
	 */
	unsigned int m_RendererID{};
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	/**
	 * \brief Bind this Vertex Buffer
	 */
	void Bind() const;
	/**
	 * \brief  Unbind this Vertex Buffer
	 */
	static void Unbind();
};
