#pragma once

/**
 * \brief OpenGL Vertex Buffer Class
 */
class IndexBuffer  // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	/**
	 * \brief Bind this Index Buffer
	 */
	void Bind() const;
	/**
	 * \brief  Unbind this Index Buffer
	 */
	static void Unbind();

	/**
	 * \brief returns the index buffer count value
	 * \return m_Count
	 */
	[[nodiscard]] unsigned int GetCount() const { return m_Count;  }
private:
	/**
	 * \brief ID of this vertex buffer
	 */
	unsigned int m_RendererID{};
	unsigned int m_Count{};
};
