#include "TestTexture2D.h"

#include "imgui.h"
#include "gtc/matrix_transform.hpp"

namespace test
{
	TestTexture2D::TestTexture2D()
		:	m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
			m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
			m_TranslationA(200, 200, 0),
			m_TranslationB(400, 400, 0)
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

		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, sizeof(positions));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, static_cast<unsigned int>(sizeof(indices)));

		m_Program = std::make_unique<GLProgram>("Basic", "res/shaders/");
		m_Program->Bind();

		m_Texture = std::make_unique<Texture>("res/textures/texture.PNG");
		m_Program->SetUniform1i("u_Texture", 0);
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
		GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Texture->Bind();

		{
			const glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			const glm::mat4 mvp = m_Proj * m_View * model;
			m_Program->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Program);
		}

		{
			const glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			const glm::mat4 mvp = m_Proj * m_View * model;
			m_Program->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Program);
		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);  // NOLINT(clang-diagnostic-double-promotion)
	}
};