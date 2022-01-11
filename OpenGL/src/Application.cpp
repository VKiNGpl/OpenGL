#include "Renderer.h"

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
	GL_CALL(const uint32_t id = glCreateShader(type));
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

static uint32_t CreateGLProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	GL_CALL(const uint32_t program = glCreateProgram());
	const uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	const uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GL_CALL(glAttachShader(program, vs));
	GL_CALL(glAttachShader(program, fs));

	GL_CALL(glLinkProgram(program));
	GL_CALL(glValidateProgram(program));

	GL_CALL(glDetachShader(program, vs));
	GL_CALL(glDetachShader(program, fs));

	return program;
}

int main(void) {
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Project", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Sync screen refresh rate
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	constexpr float positions[] = {
		-0.5f, -0.5f,	// vertex 0
		 0.5f, -0.5f,	// vertex 1
		 0.5f,  0.5f,	// vertex 2
		-0.5f,  0.5f,	// vertex 3
	};
	
	constexpr uint32_t indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	uint32_t vertex_array_obj;
	GL_CALL(glGenVertexArrays(1, &vertex_array_obj));
	GL_CALL(glBindVertexArray(vertex_array_obj));

	constexpr auto triangle_size = TRIANGLE_VERTS * VERT_SIZE;
	constexpr auto buffer_size = static_cast<uint32_t>(std::size(positions) * sizeof(positions[0]));
	constexpr auto index_buffer_size = static_cast<uint32_t>(std::size(indices) * sizeof(indices[0]));

	uint32_t buffer;
	GL_CALL(glGenBuffers(1, &buffer));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, buffer_size, positions, GL_STATIC_DRAW));

	constexpr auto vertex_stride = static_cast<uint32_t>(VERT_SIZE * sizeof(positions[0]));
	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, VERT_SIZE, GL_FLOAT, GL_FALSE, vertex_stride, nullptr));	// This is where the currently bound buffer is linked to the currently bound vertex_array_obj
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

	uint32_t index_buffer;
	GL_CALL(glGenBuffers(1, &index_buffer));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size, indices, GL_STATIC_DRAW));

	const std::string vertexShader = LoadShaderFile("res/shaders/Basic.vert");
	const std::string fragmentShader = LoadShaderFile("res/shaders/Basic.frag");

	const uint32_t gl_program = CreateGLProgram(vertexShader, fragmentShader);
	GL_CALL(glUseProgram(gl_program));

	GL_CALL(const int color_uniform = glGetUniformLocation(gl_program, "u_Color"));
	ASSERT(color_uniform != -1);
	GL_CALL(glUniform4f(color_uniform, 0.2f, 0.3f, 1.0f, 1.0f));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

		GL_CALL(glDrawElements(GL_TRIANGLES, triangle_size, GL_UNSIGNED_INT, nullptr));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	GL_CALL(glDeleteProgram(gl_program));
	glfwTerminate();

	return 0;
}
