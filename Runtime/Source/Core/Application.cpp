#include "Core/Application.h"

#include <print>

namespace Core
{
	Application::Application(const ApplicationSpecification& spec)
	{
		(void)spec;
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		std::println("Hello, World! asdasd");
	}
}