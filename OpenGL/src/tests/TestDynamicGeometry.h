#pragma once

#include "Test.h"

#include "Renderer.h"
#include "Texture.h"

namespace test
{
	struct Vertex
	{
		float Position[3];
		float Color[4];
		float TexCoords[2];
		float TexID;
	};

	class TestDynamicGeometry final : public Test  // NOLINT(cppcoreguidelines-special-member-functions)
	{
	public:
		TestDynamicGeometry();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<DynamicVertexBuffer> m_DynamicVertexBuffer;
		std::unique_ptr <IndexBuffer> m_IndexBuffer;
		std::unique_ptr <GLProgram> m_Program;
		std::unique_ptr<Texture> m_Texture, m_AltTexture;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_QuadTranslation;
		float m_QuadPosition[2];
		static std::array<Vertex, 4> CreateQuad(float x, float y, float textureID);
	};
}