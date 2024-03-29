#pragma once

#include <functional>
#include <iostream>

namespace test
{
	class Test  // NOLINT(cppcoreguidelines-special-member-functions)
	{
	public:
		Test() = default;
		virtual ~Test();

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu final : public Test
	{
	public:
		explicit TestMenu(Test*& currentTestPointer);

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
		}
	private:
		Test*& m_CurrentTest;
		std::vector < std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};
}