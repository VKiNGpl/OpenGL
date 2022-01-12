#pragma once

#include "Renderer.h"

class Texture  // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
	explicit Texture(std::string path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	static void Unbind();

	[[nodiscard]] int GetWidth() const { return m_Width; }
	[[nodiscard]] int GetHeight() const { return m_Height; }
	[[nodiscard]] int GetBPP() const { return m_BPP; }

private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
};