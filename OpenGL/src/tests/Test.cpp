#include "Test.h"

#include "imgui.h"
#include "Renderer.h"
namespace test
{
	Test::~Test()
	{
		GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		Renderer::Clear();
	}

	TestMenu::TestMenu(Test*& currentTestPointer)
		:m_CurrentTest(currentTestPointer)
	{
	}

	void TestMenu::OnImGuiRender()
	{
		for (auto& [first, second] : m_Tests)
		{
			if (ImGui::Button(first.c_str()))
				m_CurrentTest = second();
		}
		Renderer::Clear();
	}
}
