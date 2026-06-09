#include "Core/Application.h"

#include <format>

#include <CLI/CLI.hpp>

namespace Core
{
	Application::Application(const ApplicationSpecification& spec, int argc, char** argv)
		: m_Running(true)
	{
		EventBus::Get().AddListener(this);

		CLI::App app{ std::format("{} - {}", spec.Name, spec.Description) };
		argv = app.ensure_utf8(argv);

		try
		{
			app.parse(argc, argv);
		}
		catch (const CLI::ParseError& err)
		{
			(void)err;
		}

		m_Window = Window::Create(spec.WindowSpec);
	}

	Application::~Application()
	{
		EventBus::Get().RemoveListener(this);
	}

	void Application::Run()
	{
		while (m_Running)
		{
			EventBus::Get().ProcessEvents();

			/* Poll events */
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(const Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>([this]([[maybe_unused]] const WindowCloseEvent& e) { m_Running = false; });
	}
}