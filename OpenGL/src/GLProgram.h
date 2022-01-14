#pragma once

#include <string>
#include <unordered_map>

#include "glm.hpp"

class GLProgram  // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
	GLProgram(std::string filename, std::string filepath);
	~GLProgram();

	/**
	 * \brief Sets up a Vec1 int uniform\n
	 * \param name name of the uniform
	 * \param value attribute 0 of uniform (int)
	 */
	void SetUniform1i(const std::string& name, int value) const;
	/**
	 * \brief Sets up a Vec1 float uniform\n
	 * \param name name of the uniform
	 * \param value attribute 0 of uniform (float)
	 */
	void SetUniform1f(const std::string& name, float value) const;
	/**
	 * \brief Sets up a Vec1 float uniform\n
	 * \param name name of the uniform
	 * \param value vector 2 argument
	 */
	void SetUniform2f(const std::string& name, const glm::vec2& value) const;
	/**
	 * \brief Sets up a Vec1 float uniform\n
	 * \param name name of the uniform
	 * \param value vector 3 argument
	 */
	void SetUniform3f(const std::string& name, const glm::vec3& value) const;
	/**
	 * \brief Sets up a Mat4 float uniform\n
	 * \param name name of the uniform
	 * \param value vector 4 argument
	 */
	void SetUniform4f(const std::string& name, const glm::vec4& value) const;
	 /**
	  * \brief Sets up a Vec4 float uniform\n
	  * \param name name of the uniform
	  * \param matrix mat3 projection matrix
	  */
	void SetUniformMat3f(const std::string& name, const glm::mat3& matrix) const;
	 /**
	  * \brief Sets up a Vec4 float uniform\n
	  * \param name name of the uniform
	  * \param matrix mat4 projection matrix
	  */
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const;

	void Bind() const;
	static void Unbind();
private:
	std::string m_FileName;
	std::string m_FilePath;
	unsigned int m_RenderID;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;

	static std::string  LoadShaderFile(const std::string& filename, const std::string& filepath);
	static unsigned int CompileShader(unsigned int type, const std::string& source);
	static unsigned int CreateGLProgram(const std::string& vertexShader, const std::string& fragmentShader);

	[[nodiscard]] int GetUniformLocation(const std::string& name) const;
};
