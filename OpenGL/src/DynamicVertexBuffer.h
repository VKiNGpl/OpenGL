#pragma once

/**
 * \brief OpenGL Vertex Buffer Class
 */
class DynamicVertexBuffer  // NOLINT(cppcoreguidelines-special-member-functions)
{
private:
	/**
	 * \brief ID of this vertex buffer
	 */
	unsigned int m_RendererID{};
public:
	DynamicVertexBuffer(unsigned int size);
	~DynamicVertexBuffer();

	/**
	 * \brief Bind this Vertex Buffer
	 */
	void Bind() const;
	/**
	 * \brief  Unbind this Vertex Buffer
	 */
	static void Unbind();
};
