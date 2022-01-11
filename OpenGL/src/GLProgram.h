#pragma once

#include <string>
#include <unordered_map>

class GLProgram  // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
	GLProgram(std::string filename, std::string filepath);
	~GLProgram();

	/**
	 * \brief Sets up a Vec4 uniform\n
	 * \param name name of the uniform
	 * \param v0 attribute 0 of uniform
	 * \param v1 attribute 1 of uniform
	 * \param v2 attribute 2 of uniform
	 * \param v3 attribute 3 of uniform
	 */
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	/**
	 * \brief Sets up a Vec1 uniform\n
	 * \param name name of the uniform
	 * \param value attribute 0 of uniform
	 */
	void SetUniform1f(const std::string& name, float value);

	void Bind() const;
	static void Unbind();
private:
	std::string m_FileName;
	std::string m_FilePath;
	unsigned int m_RenderID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	static std::string  LoadShaderFile(const std::string& filename, const std::string& filepath);
	static unsigned int CompileShader(unsigned int type, const std::string& source);
	static unsigned int CreateGLProgram(const std::string& vertexShader, const std::string& fragmentShader);

	[[nodiscard]] int GetUniformLocation(const std::string& name);
};
