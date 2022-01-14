#pragma once

#include "Test.h"

#include "Renderer.h"

namespace test
{
	class TestColorQuad final : public Test  // NOLINT(cppcoreguidelines-special-member-functions)
	{
	public:
		TestColorQuad();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr <IndexBuffer> m_IndexBuffer;
		std::unique_ptr <GLProgram> m_Program;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_QuadTranslation;
	};
}