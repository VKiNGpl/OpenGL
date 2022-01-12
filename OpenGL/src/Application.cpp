#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "GLProgram.h"
#include "Texture.h"

#include <iostream>

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

		GLProgram program("Basic", "res/shaders/");

		const VertexArray va;
		const VertexBuffer vb(positions, sizeof(positions));

		VertexBufferLayout layout;
		layout.Push<float>(VERT_SIZE);
		va.AddBuffer(vb, layout);

		const IndexBuffer ib(indices, sizeof(indices));

		const Texture texture("res/textures/texture.PNG");
		texture.Bind();

		Renderer renderer;

		float red = 0.2f;
		float step = 0.01f;
		const float increment = step;
		const float decrement = -increment;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			Renderer::Clear();

			program.Bind();
			program.SetUniform4f("u_Color", red, 0.3f, 0.8f, 1.0f);

			renderer.Draw(va, ib, program);

			if (red > 1.0f)
				step = decrement;
			else if (red < 0.2f)
				step = increment;

			red += step;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate();

	return 0;
}
