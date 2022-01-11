#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

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

static unsigned int CompileShader(const unsigned int type, const std::string& source)
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

static unsigned int CreateGLProgram(const std::string& vertexShader, const std::string& fragmentShader)
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
	{
		constexpr float positions[] = {
			-0.5f, -0.5f,	// vertex 0
			 0.5f, -0.5f,	// vertex 1
			 0.5f,  0.5f,	// vertex 2
			-0.5f,  0.5f,	// vertex 3
		};

		constexpr unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		constexpr auto triangle_size = TRIANGLE_VERTS * VERT_SIZE;

		const VertexArray va;
		const VertexBuffer vb(positions, sizeof(positions));

		VertexBufferLayout layout;
		layout.Push<float>(VERT_SIZE);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, sizeof(indices));

		const std::string vertexShader = LoadShaderFile("res/shaders/Basic.vert");
		const std::string fragmentShader = LoadShaderFile("res/shaders/Basic.frag");

		const unsigned int gl_program = CreateGLProgram(vertexShader, fragmentShader);
		GL_CALL(glUseProgram(gl_program));

		GL_CALL(const int color_uniform = glGetUniformLocation(gl_program, "u_Color"));
		ASSERT(color_uniform != -1);
		GL_CALL(glUniform4f(color_uniform, 0.2f, 0.3f, 1.0f, 1.0f));

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

			/*va.Bind();
			ib.Bind();*/

			GL_CALL(glDrawElements(GL_TRIANGLES, triangle_size, GL_UNSIGNED_INT, nullptr));

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		GL_CALL(glDeleteProgram(gl_program));
	}
	glfwTerminate();

	return 0;
}
