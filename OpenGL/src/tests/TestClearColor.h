#pragma once

#include "test.h"

namespace test
{
	class TestClearColor final : public Test  // NOLINT(cppcoreguidelines-special-member-functions)
	{
	public:
		TestClearColor();
		~TestClearColor() override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		float m_ClearColor[4];
	};
}