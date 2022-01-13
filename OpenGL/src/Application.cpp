#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

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
	GLFWwindow* window = glfwCreateWindow(960, 540, "OpenGL Project", nullptr, nullptr);
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
			-50.0f, -50.0f, 0.0f, 0.0f,	// vertex 0
			 50.0f, -50.0f, 1.0f, 0.0f,	// vertex 1
			 50.0f,  50.0f, 1.0f, 1.0f,	// vertex 2
			-50.0f,  50.0f, 0.0f, 1.0f	// vertex 3
		};

		constexpr unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GL_CALL(glEnable(GL_BLEND));
		GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		const VertexArray va;
		const VertexBuffer vb(positions, sizeof(positions));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		const IndexBuffer ib(indices, sizeof(indices));

		const glm::mat4 proj  = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		const glm::mat4 view  = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f ,0.0f ));

		GLProgram program("Basic", "res/shaders/");
		program.Bind();

		const Texture texture("res/textures/texture.PNG");
		texture.Bind();
		program.SetUniform1i("u_Texture", 0);	// value parameter must match the currently bound texture slot

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		glm::vec3 translationA(200.0f, 200.0f, 0.0f);
		glm::vec3 translationB(400.0f, 200.0f, 0.0f);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			Renderer::Clear();

			ImGui_ImplGlfwGL3_NewFrame();

			{
				const glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
				const glm::mat4 mvp = proj * view * model;
				program.SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(va, ib, program);
			}

			{
				const glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
				const glm::mat4 mvp = proj * view * model;
				program.SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(va, ib, program);
			}

			{
				ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
				ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);  // NOLINT(clang-diagnostic-double-promotion)
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}
