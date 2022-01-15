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
			m_CamPosition(200, 200, 0),
			m_QuadPosition{ -150.0f, -50.0f }
	{
		constexpr size_t maxQuadCount = 400;
		constexpr size_t maxVertexCount = maxQuadCount * 4;
		constexpr size_t maxIndexCount = maxQuadCount * 6;

		uint32_t indices[maxIndexCount]{};
		uint32_t offset = 0;
		for (size_t i = 0; i < maxIndexCount; i +=6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		m_VAO = std::make_unique<VertexArray>();

		m_DynamicVertexBuffer = std::make_unique<DynamicVertexBuffer>(static_cast<unsigned int>(maxVertexCount * sizeof(Vertex)));

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
		std::array<Vertex, 400> vertices{};
		Vertex* buffer = vertices.data();

		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < 5; x++)
			{
				buffer = CreateQuad(buffer, static_cast<float>(x*100), static_cast<float>(y*100), static_cast<float>((x + y) % 2));
			}
		}

		CreateQuad(buffer, m_QuadPosition.x, m_QuadPosition.y, 0.0f);

		m_DynamicVertexBuffer->Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		GL_CALL(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

		const glm::mat4 model = glm::translate(glm::mat4(1.0f), m_CamPosition);
		const glm::mat4 mvp = m_Proj * m_View * model;

		m_Program->SetUniformMat4f("u_MVP", mvp);
		
		Renderer renderer;
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Program);
	}

	void TestDynamicGeometry::OnImGuiRender()
	{
		ImGui::DragFloat2("Quad Position", &m_QuadPosition.x, 1.0f);
		ImGui::SliderFloat3("Quad Translation", &m_CamPosition.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);  // NOLINT(clang-diagnostic-double-promotion)
	}

	TestDynamicGeometry::Vertex* TestDynamicGeometry::CreateQuad(Vertex* target, float x, float y, const float textureID)
	{
		constexpr float size = 100.0f;

		target->Position = { x, y, 0.0f };
		target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		target->TexCoords = { 0.0f, 0.0f };
		target->TexID = textureID;
		target++;

		target->Position = { x + size, y, 0.0f };
		target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		target->TexCoords = { 1.0f, 0.0f };
		target->TexID = textureID;
		target++;

		target->Position = { x + size, y + size, 0.0f };
		target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		target->TexCoords = { 1.0f, 1.0f };
		target->TexID = textureID;
		target++;

		target->Position = { x, y + size, 0.0f };
		target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		target->TexCoords = { 0.0f, 1.0f };
		target->TexID = textureID;
		target++;

		return target;
	}
};