#include "TestDynamicGeometry.h"
#include "VertexBufferLayout.h"

#include "imgui.h"
#include "gtc/matrix_transform.hpp"

#include <array>

namespace test
{
	TestDynamicGeometry::TestDynamicGeometry()
		:	m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
			m_View(translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
			m_QuadTranslation(200, 200, 0),
			m_QuadPosition{ -150.0f, -50.0f }
	{
		constexpr unsigned int indices[] = {	// can setup a for loop to generate max number of vertices
			0, 1, 2,	// Quad 1 (Triangle 1/2)
			2, 3, 0,	// Quad 1 (Triangle 2/2)

			4, 5, 6,	// Quad 2 (Triangle 1/2)
			6, 7, 4		// Quad 2 (Triangle 2/2)
		};

		m_VAO = std::make_unique<VertexArray>();

		m_DynamicVertexBuffer = std::make_unique<DynamicVertexBuffer>(static_cast<unsigned int>(sizeof(Vertex) * 1000));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(4);
		layout.Push<float>(2);
		layout.Push<float>(1);

		m_VAO->AddDynamicBuffer(*m_DynamicVertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, static_cast<unsigned int>(sizeof(indices)));

		m_Program = std::make_unique<GLProgram>("BasicDynamicGeometry", "res/shaders/");
		m_Program->Bind();

		m_Texture = std::make_unique<Texture>("res/textures/texture.PNG");
		m_AltTexture = std::make_unique<Texture>("res/textures/alt_texture.PNG");
		m_Texture->Bind();
		m_AltTexture->Bind(1);

		const auto loc = glGetUniformLocation(m_Program->GetRendererID(), "u_Textures");
		constexpr int samplers[2] = { 0, 1 };
		glUniform1iv(loc, 2, samplers);
	}

	void TestDynamicGeometry::OnUpdate(float deltaTime)
	{
	}

	void TestDynamicGeometry::OnRender()
	{
		// Set dynamic vertex buffer
		//float vertices[] = {
		//	-150.0f, -50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f,	// Quad 1 vertex 0 (vec2 pos, vec2 texture coordinate)
		//	 -50.0f, -50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,	// Quad 1 vertex 1 (vec2 pos, vec2 texture coordinate)
		//	 -50.0f,  50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,	// Quad 1 vertex 2 (vec2 pos, vec2 texture coordinate)
		//	-150.0f,  50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,	// Quad 1 vertex 3 (vec2 pos, vec2 texture coordinate)

		//	  50.0f, -50.0f, 0.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f,	// Quad 2 vertex 0 (vec2 pos, vec2 texture coordinate)
		//	 150.0f, -50.0f, 1.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f,	// Quad 2 vertex 1 (vec2 pos, vec2 texture coordinate)
		//	 150.0f,  50.0f, 1.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f,	// Quad 2 vertex 2 (vec2 pos, vec2 texture coordinate)
		//	  50.0f,  50.0f, 0.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f	// Quad 2 vertex 3 (vec2 pos, vec2 texture coordinate)
		//};

		const auto q0 = CreateQuad(m_QuadPosition[0], m_QuadPosition[1], 0.0f);
		const auto q1 = CreateQuad(50.0f, -50.0f, 1.0f);

		Vertex vertices[8];

		memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
		memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));

		m_DynamicVertexBuffer->Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		GL_CALL(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;


		const glm::mat4 model = glm::translate(glm::mat4(1.0f), m_QuadTranslation);
		const glm::mat4 mvp = m_Proj * m_View * model;

		m_Program->SetUniformMat4f("u_MVP", mvp);
		
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Program);
	}

	void TestDynamicGeometry::OnImGuiRender()
	{
		ImGui::DragFloat2("Quad Position", m_QuadPosition, 0.1f);
		ImGui::SliderFloat3("Quad Translation", &m_QuadTranslation.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);  // NOLINT(clang-diagnostic-double-promotion)
	}

	std::array<Vertex, 4> TestDynamicGeometry::CreateQuad(const float x, const float y, const float textureID)
	{
		constexpr float size = 100.0f;

		const Vertex v0 = { {x, y, 0.0f}, {0.18f, 0.6f, 0.96f, 1.0f}, {0.0f, 0.0f}, textureID };
		const Vertex v1 = { {x + size, y, 0.0f}, {0.18f, 0.6f, 0.96f, 1.0f}, {1.0f, 0.0f}, textureID };
		const Vertex v2 = { {x + size, y + size, 0.0f}, {0.18f, 0.6f, 0.96f, 1.0f}, {1.0f, 1.0f}, textureID };
		const Vertex v3 = { {x, y + size, 0.0f}, {0.18f, 0.6f, 0.96f, 1.0f}, {0.0f, 1.0f}, textureID };

		return { v0, v1, v2, v3 };
	}

};