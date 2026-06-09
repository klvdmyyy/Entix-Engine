#include "Core/Application.h"

#include <print>

namespace Core
{
	Application::Application(const ApplicationSpecification& spec)
	{
		m_Window = Window::Create(spec.WindowSpec);
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			/* Poll events */
			m_Window->OnUpdate();
		}
	}
}