#pragma once

#include "Test.h"

#include "Renderer.h"
#include "Texture.h"

namespace test
{
	class TestBatchRenderingTexture2D final : public Test  // NOLINT(cppcoreguidelines-special-member-functions)
	{
	public:
		TestBatchRenderingTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr <IndexBuffer> m_IndexBuffer;
		std::unique_ptr <GLProgram> m_Program;
		std::unique_ptr<Texture> m_Texture, m_AltTexture;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_TranslationA, m_TranslationB;
	};
}