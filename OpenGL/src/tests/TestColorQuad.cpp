#include "TestColorQuad.h"
#include "VertexBufferLayout.h"

#include "imgui.h"
#include "gtc/matrix_transform.hpp"

namespace test
{
	TestColorQuad::TestColorQuad()
		:	m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
			m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
			m_QuadTranslation(200, 200, 0)
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

		m_VAO = std::make_unique<VertexArray>();

		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, static_cast<unsigned int>(sizeof(positions)));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, static_cast<unsigned int>(sizeof(indices)));

		m_Program = std::make_unique<GLProgram>("BasicColorQuad", "res/shaders/");
		m_Program->Bind();
	}

	void TestColorQuad::OnUpdate(float deltaTime)
	{
	}

	void TestColorQuad::OnRender()
	{
		GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));


		const glm::mat4 model = glm::translate(glm::mat4(1.0f), m_QuadTranslation);
		const glm::mat4 mvp = m_Proj * m_View * model;

		m_Program->SetUniform4f("u_Color", { 0.8f, 0.3f, 0.2f, 1.0f });
		m_Program->SetUniformMat4f("u_MVP", mvp);

		Renderer renderer;
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Program);
	}

	void TestColorQuad::OnImGuiRender()
	{
		ImGui::SliderFloat3("Quad Translation", &m_QuadTranslation.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);  // NOLINT(clang-diagnostic-double-promotion)
	}
};