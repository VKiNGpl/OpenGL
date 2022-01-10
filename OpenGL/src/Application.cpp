#include "Application.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

static std::string LoadShaderFile(const char* filepath)
{
	const std::ifstream stream(filepath);
	std::stringstream buffer;

	buffer << stream.rdbuf();

	return buffer.str();
}

static uint32_t CompileShader(const uint32_t type, const std::string& source)
{
	const uint32_t id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int32_t result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int32_t length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		const auto message = static_cast<char*>(_malloca(length * sizeof(char)));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile " << 
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
			" shader" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);

		return 0;
	}

	return id;
}

static uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	const uint32_t program = glCreateProgram();
	const uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	const uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDetachShader(program, vs);
	glDetachShader(program, fs);

	return program;
}

int main(void) {
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	constexpr float positions[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f, 
		 0.5f,  0.5f,
		-0.5f,  0.5f
	};

	const uint32_t indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	constexpr auto triangle_size = TRIANGLE_VERTS * VERT_SIZE;
	constexpr auto triangles_count = std::size(indices) / TRIANGLE_VERTS;
	constexpr auto buffer_size = triangle_size * triangles_count * sizeof(positions[0]);
	constexpr auto index_buffer_size = buffer_size / triangles_count;

	uint32_t buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, positions, GL_STATIC_DRAW);

	constexpr auto vertex_offset = VERT_SIZE * sizeof(positions[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vertex_offset, nullptr);

	uint32_t index_buffer;
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size, indices, GL_STATIC_DRAW);

	const std::string vertexShader = LoadShaderFile("res/shaders/Basic.vert");
	const std::string fragmentShader = LoadShaderFile("res/shaders/Basic.frag");

	const uint32_t shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, triangle_size, GL_UNSIGNED_INT, nullptr);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();

	return 0;
}
