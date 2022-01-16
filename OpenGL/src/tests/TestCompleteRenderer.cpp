#include "TestCompleteRenderer.h"

#include "imgui.h"
#include "gtc/matrix_transform.hpp"

#include <array>

namespace test
{
	static constexpr size_t MaxQuadCount = 650;
	static constexpr size_t MaxVertexCount = MaxQuadCount * 4;
	static constexpr size_t MaxIndexCount = MaxQuadCount * 6;
	static constexpr size_t MaxTextures = 32;

	struct BatchRendererData
	{
		GLuint QuadVA = 0;
		GLuint QuadVB = 0;
		GLuint QuadIB = 0;

		GLuint WhiteTexture = 0;
		uint32_t WhiteTextureSlot = 0;

		uint32_t IndexCount = 0;

		TestCompleteRenderer::Vertex* QuadBuffer = nullptr;
		TestCompleteRenderer::Vertex* QuadBufferPtr = nullptr;

		std::array<uint32_t, MaxTextures> TextureSlots{};
		uint32_t TextureSlotIndex = 1;

		TestCompleteRenderer::Stats RenderStats;
	};

	static BatchRendererData s_Data;

	TestCompleteRenderer::TestCompleteRenderer()
		:	m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
			m_View(translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
			m_CamPosition(200, 200, 0),
			m_QuadPosition{ -150.0f, -50.0f }
	{
		s_Data.QuadBuffer = new Vertex[MaxVertexCount];

		glCreateVertexArrays(1, &s_Data.QuadVA);
		glBindVertexArray(s_Data.QuadVA);

		glCreateBuffers(1, &s_Data.QuadVB);
		glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB);
		glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexArrayAttrib(s_Data.QuadVA, 0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, Position)));  // NOLINT(performance-no-int-to-ptr)

		glEnableVertexArrayAttrib(s_Data.QuadVA, 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, Color)));  // NOLINT(performance-no-int-to-ptr)

		glEnableVertexArrayAttrib(s_Data.QuadVA, 0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, TexCoords)));  // NOLINT(performance-no-int-to-ptr)

		glEnableVertexArrayAttrib(s_Data.QuadVA, 0);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, TexIndex)));  // NOLINT(performance-no-int-to-ptr)

		uint32_t indices[MaxIndexCount]{};
		uint32_t offset = 0;
		for (size_t i = 0; i < MaxIndexCount; i +=6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		glCreateBuffers(1, &s_Data.QuadIB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.QuadIB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		
		m_Program = std::make_unique<GLProgram>("BasicBatchRenderer", "res/shaders/");
		m_Program->Bind();

		m_Texture = std::make_unique<Texture>("res/textures/texture.PNG");
		m_AltTexture = std::make_unique<Texture>("res/textures/alt_texture.PNG");
		m_Texture->Bind();
		m_AltTexture->Bind(1);

		// 1x1 white texture
		glCreateTextures(GL_TEXTURE_2D, 1, &s_Data.WhiteTexture);
		glBindTexture(GL_TEXTURE_2D, s_Data.WhiteTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		constexpr uint32_t color = 0xffffffff;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
		for (size_t i = 1; i < MaxTextures; i++)
			s_Data.TextureSlots[i] = 0;

		const auto loc = glGetUniformLocation(m_Program->GetRendererID(), "u_Textures");
		constexpr int samplers[2] = { 0, 1 };
		glUniform1iv(loc, 2, samplers);

		glUseProgram(m_Program->GetRendererID());
	}

	void TestCompleteRenderer::OnUpdate(float deltaTime)
	{
	}

	void TestCompleteRenderer::OnRender()
	{
		//GL_CALL(glClearColor(1.f, 0.1f, 0.1f, 1.0f));
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
		glUseProgram(m_Program->GetRendererID());

		ResetStats();
		BeginBatch();

		for (float y = -10.0f; y < 10.0f; y += 0.25f)  // NOLINT(cert-flp30-c)
		{
			for (float x = -10.0f; x < 10.0f; x += 0.25f)  // NOLINT(cert-flp30-c)
			{
				glm::vec2 position = { x, y };
				glm::vec4 color = { (x + 10) / 20.0f, 0.2f, (y+10) / 20.0f, 1.0f };
				DrawQuad(position, { 100.0f, 100.0f }, color);
			}
		}

		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < 5; x++)
			{
				glm::vec2 position = { static_cast<float>(x), static_cast<float>(y) };
				DrawQuad(position, { 10.0f, 10.0f }, static_cast<uint32_t>((x + y) % 2));
			}
		}

		DrawQuad(m_QuadPosition, { 100.0f, 100.0f }, 1);

		const glm::mat4 model = glm::translate(glm::mat4(1.0f), m_CamPosition);
		const glm::mat4 mvp = m_Proj * m_View * model;

		m_Program->SetUniformMat4f("u_ViewProj", mvp);

		EndBatch();
		Flush();
	}

	void TestCompleteRenderer::OnImGuiRender()
	{
		ImGui::DragFloat2("Quad Position", &m_QuadPosition.x, 1.0f);
		ImGui::SliderFloat3("Quad Translation", &m_CamPosition.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);  // NOLINT(clang-diagnostic-double-promotion)
	}

	void TestCompleteRenderer::Shutdown()
	{
		glDeleteVertexArrays(1, &s_Data.QuadVA);
		glDeleteBuffers(1, &s_Data.QuadVB);
		glDeleteBuffers(1, &s_Data.QuadIB);

		glDeleteTextures(1, &s_Data.WhiteTexture);

		delete[] s_Data.QuadBuffer;
	}

	void TestCompleteRenderer::BeginBatch()
	{
		s_Data.QuadBufferPtr = s_Data.QuadBuffer;
	}

	void TestCompleteRenderer::EndBatch()
	{
		const GLsizeiptr size = reinterpret_cast<uint8_t*>(s_Data.QuadBufferPtr) - reinterpret_cast<uint8_t*>(s_Data.QuadBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.QuadBuffer);
	}

	void TestCompleteRenderer::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			glBindTextureUnit(i, s_Data.TextureSlots[i]);

		glBindVertexArray(s_Data.QuadVA);
		glDrawElements(GL_TRIANGLES, static_cast<int>(s_Data.IndexCount), GL_UNSIGNED_INT, nullptr);
		s_Data.RenderStats.DrawCount++;

		s_Data.IndexCount = 0;
		s_Data.TextureSlotIndex = 1;
	}

	void TestCompleteRenderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (s_Data.IndexCount >= MaxIndexCount)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		constexpr float textureIndex = 0.0f;

		s_Data.QuadBufferPtr->Position = { position.x,  position.y, 0.0f };
		s_Data.QuadBufferPtr->Color = color;
		s_Data.QuadBufferPtr->TexCoords = { 0.0f, 0.0f };
		s_Data.QuadBufferPtr->TexIndex = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.QuadBufferPtr->Position = { position.x + size.x,  position.y, 0.0f };
		s_Data.QuadBufferPtr->Color = color;
		s_Data.QuadBufferPtr->TexCoords = { 1.0f, 0.0f };
		s_Data.QuadBufferPtr->TexIndex = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.QuadBufferPtr->Position = { position.x + size.x,  position.y + size.y, 0.0f };
		s_Data.QuadBufferPtr->Color = color;
		s_Data.QuadBufferPtr->TexCoords = { 1.0f, 1.0f };
		s_Data.QuadBufferPtr->TexIndex = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.QuadBufferPtr->Position = { position.x,  position.y + size.y, 0.0f };
		s_Data.QuadBufferPtr->Color = color;
		s_Data.QuadBufferPtr->TexCoords = { 0.0f, 1.0f };
		s_Data.QuadBufferPtr->TexIndex = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.IndexCount += 6;
		s_Data.RenderStats.QuadCount++;
	}

	void TestCompleteRenderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const uint32_t textureID)
	{
		if (s_Data.IndexCount >= MaxIndexCount || s_Data.TextureSlotIndex > 31)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i] == textureID)
			{
				textureIndex = static_cast<float>(i);
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = textureID;
			s_Data.TextureSlotIndex++;
		}

		s_Data.QuadBufferPtr->Position = { position.x, position.y, 0.0f };
		s_Data.QuadBufferPtr->Color = color;
		s_Data.QuadBufferPtr->TexCoords = { 0.0f, 0.0f };
		s_Data.QuadBufferPtr->TexIndex = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.QuadBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadBufferPtr->Color = color;
		s_Data.QuadBufferPtr->TexCoords = { 1.0f, 0.0f };
		s_Data.QuadBufferPtr->TexIndex = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.QuadBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadBufferPtr->Color = color;
		s_Data.QuadBufferPtr->TexCoords = { 1.0f, 1.0f };
		s_Data.QuadBufferPtr->TexIndex = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.QuadBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadBufferPtr->Color = color;
		s_Data.QuadBufferPtr->TexCoords = { 0.0f, 1.0f };
		s_Data.QuadBufferPtr->TexIndex = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.IndexCount += 6;
		s_Data.RenderStats.QuadCount++;
	}

	const test::TestCompleteRenderer::Stats& TestCompleteRenderer::GetStats()
	{
		return s_Data.RenderStats;
	}

	void TestCompleteRenderer::ResetStats()
	{
		memset(&s_Data.RenderStats, 0, sizeof(Stats));
	}

};