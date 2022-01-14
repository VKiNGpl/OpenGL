#pragma once

namespace test
{

	class Test  // NOLINT(cppcoreguidelines-special-member-functions)
	{
	public:
		Test() = default;
		virtual ~Test() = default;

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};
}