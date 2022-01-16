#pragma once

#include "Test.h"

#include "Renderer.h"
#include "Texture.h"

namespace test
{
	class TestCompleteRenderer final : public Test  // NOLINT(cppcoreguidelines-special-member-functions)
	{
	public:
		TestCompleteRenderer();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		struct Vertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoords;
			float TexIndex;
		};

		// Stats
		struct Stats
		{
			uint32_t DrawCount = 0;
			uint32_t QuadCount = 0;
		};
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<DynamicVertexBuffer> m_DynamicVertexBuffer;
		std::unique_ptr <IndexBuffer> m_IndexBuffer;
		std::unique_ptr <GLProgram> m_Program;
		std::unique_ptr<Texture> m_Texture, m_AltTexture;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_CamPosition;
		glm::vec2 m_QuadPosition;


		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureID);

		static void Shutdown();

		static void BeginBatch();
		static void EndBatch();
		static void Flush();

		static const Stats& GetStats();
		static void ResetStats();
	};
}