#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(std::string filename, std::string filepath)
	: m_FileName(std::move(filename)), m_FilePath(std::move(filepath)), m_RenderID(0)
{
	const std::string vertexShader = LoadShaderFile(m_FileName + ".vert", m_FilePath);
	const std::string fragmentShader = LoadShaderFile(m_FileName + ".frag", m_FilePath);

	m_RenderID = CreateGLProgram(vertexShader, fragmentShader);
}

Shader::~Shader()
{
	GL_CALL(glDeleteProgram(m_RenderID));
}

std::string Shader::LoadShaderFile(const std::string& filename, const std::string& filepath)
{
	const std::ifstream stream(filepath + filename);
	std::stringstream buffer;

	buffer << stream.rdbuf();

	return buffer.str();
}

unsigned int Shader::CompileShader(const unsigned int type, const std::string& source)
{
	GL_CALL(const unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GL_CALL(glShaderSource(id, 1, &src, nullptr));
	GL_CALL(glCompileShader(id));

	int32_t result;
	GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int32_t length;
		GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		const auto message = static_cast<char*>(_malloca(length * sizeof(char)));
		GL_CALL(glGetShaderInfoLog(id, length, &length, message));

		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
			" shader" << std::endl;
		std::cout << message << std::endl;

		GL_CALL(glDeleteShader(id));

		return 0;
	}

	return id;
}

unsigned int Shader::CreateGLProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	GL_CALL(const unsigned int program = glCreateProgram());
	const unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	const unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GL_CALL(glAttachShader(program, vs));
	GL_CALL(glAttachShader(program, fs));

	GL_CALL(glLinkProgram(program));
	GL_CALL(glValidateProgram(program));

	GL_CALL(glDetachShader(program, vs));
	GL_CALL(glDetachShader(program, fs));

	return program;
}

void Shader::SetUniform4f(const std::string& name, const float v0, const float v1, const float v2, const float v3)
{
	GL_CALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1f(const std::string& name, const float value)
{
	GL_CALL(glUniform1f(GetUniformLocation(name), value));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GL_CALL(const int location = glGetUniformLocation(m_RenderID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' does not exist!" << std::endl;

	m_UniformLocationCache[name] = location;
	return location;
}

void Shader::Bind() const
{
	GL_CALL(glUseProgram(m_RenderID));
}

void Shader::Unbind()
{
	GL_CALL(glUseProgram(0));
}
