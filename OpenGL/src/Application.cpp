#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestColorQuad.h"
#include "tests/TestBatchRendering.h"
#include "tests/TestBatchRenderigColor.h"
#include "tests/TestBatchRenderingTexture2D.h"
#include "tests/TestDynamicGeometry.h"

int main(void) {
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(960, 540, "OpenGL Project", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	test::Test* currentTest = nullptr;
	const auto testMenu = new test::TestMenu(currentTest);
	currentTest = testMenu;

	testMenu->RegisterTest<test::TestClearColor>("Clear Color");
	testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
	testMenu->RegisterTest<test::TestColorQuad>("Color Quad");
	testMenu->RegisterTest<test::TestBatchRendering>("Batch Rendering");
	testMenu->RegisterTest<test::TestBatchRenderingColor>("Batch Rendering Color");
	testMenu->RegisterTest<test::TestBatchRenderingTexture2D>("Batch 2D Texture");
	testMenu->RegisterTest<test::TestDynamicGeometry>("Dynamic Geometry");

	while (!glfwWindowShouldClose(window))
	{
		ImGui_ImplGlfwGL3_NewFrame();
		if (currentTest)
		{
			currentTest->OnUpdate(0.0f);
			currentTest->OnRender();

			ImGui::Begin("Test");
			if (currentTest != testMenu && ImGui::Button("<-"))
			{
				delete currentTest;
				currentTest = testMenu;
			}
			currentTest->OnImGuiRender();
			ImGui::End();
		}
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	if (currentTest != testMenu)
		delete testMenu;
	delete currentTest;

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}
