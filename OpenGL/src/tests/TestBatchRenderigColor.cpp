#include "TestBatchRenderigColor.h"
#include "VertexBufferLayout.h"

#include "imgui.h"
#include "gtc/matrix_transform.hpp"

namespace test
{
	TestBatchRenderingColor::TestBatchRenderingColor()
		:	m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
			m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
			m_QuadTranslation(480, 270, 0)
	{
		constexpr float positions[] = {
			-150.0f, -50.0f, 0.0f,	0.18f, 0.6f, 0.96f, 1.0f,	// Quad 1 vertex 0 (vec3 pos, vec4 color)
			 -50.0f, -50.0f, 0.0f,	0.18f, 0.6f, 0.96f, 1.0f,	// Quad 1 vertex 1 (vec3 pos, vec4 color)
			 -50.0f,  50.0f, 0.0f,	0.18f, 0.6f, 0.96f, 1.0f,	// Quad 1 vertex 2 (vec3 pos, vec4 color)
			-150.0f,  50.0f, 0.0f,	 0.0f, 0.0f,  0.0f, 1.0f,	// Quad 1 vertex 3 (vec3 pos, vec4 color)

			  50.0f, -50.0f, 0.0f,	1.0f, 0.93f, 0.24f, 1.0f,	// Quad 2 vertex 0 (vec3 pos, vec4 color)
			 150.0f, -50.0f, 0.0f,	1.0f, 0.93f, 0.24f, 1.0f,	// Quad 2 vertex 1 (vec3 pos, vec4 color)
			 150.0f,  50.0f, 0.0f,	0.0f,  0.0f,  0.0f, 1.0f,	// Quad 2 vertex 2 (vec3 pos, vec4 color)
			  50.0f,  50.0f, 0.0f,	1.0f, 0.93f, 0.24f, 1.0f	// Quad 2 vertex 3 (vec3 pos, vec4 color)
		};

		constexpr unsigned int indices[] = {
			0, 1, 2,	// Quad 1 (Triangle 1/2)
			2, 3, 0,	// Quad 1 (Triangle 2/2)

			4, 5, 6,	// Quad 2 (Triangle 1/2)
			6, 7, 4		// Quad 2 (Triangle 2/2)
		};

		m_VAO = std::make_unique<VertexArray>();

		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, static_cast<unsigned int>(sizeof(positions)));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(4);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, static_cast<unsigned int>(sizeof(indices)));

		m_Program = std::make_unique<GLProgram>("BasicBatchColor", "res/shaders/");
		m_Program->Bind();
	}

	void TestBatchRenderingColor::OnUpdate(float deltaTime)
	{
	}

	void TestBatchRenderingColor::OnRender()
	{
		GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

		const glm::mat4 model = glm::translate(glm::mat4(1.0f), m_QuadTranslation);
		const glm::mat4 mvp = m_Proj * m_View * model;

		m_Program->SetUniformMat4f("u_MVP", mvp);

		Renderer renderer;
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Program);
	}

	void TestBatchRenderingColor::OnImGuiRender()
	{
		ImGui::SliderFloat3("Quad Translation", &m_QuadTranslation.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);  // NOLINT(clang-diagnostic-double-promotion)
	}
};