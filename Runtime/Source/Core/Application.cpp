#include "Core/Application.h"

#include <format>

#include <CLI/CLI.hpp>

#include "Core/Log.h"

namespace Core
{
	Application::Application(const ApplicationSpecification& spec, int argc, char** argv)
		: m_Running(true)
	{
		Log::Init();
		E_INFO("Initialized Log!");

		EventBus::Get().AddListener(this);

		CLI::App app{ std::format("{} - {}", spec.Name, spec.Description) };
		argv = app.ensure_utf8(argv);

		try
		{
			app.parse(argc, argv);
		}
		catch (const CLI::ParseError& err)
		{
			/* Just ignore errors here. */
			(void)err;
		}

		this->Init();
		m_Window = Window::Create(spec.WindowSpec);
	}

	Application::~Application()
	{
		this->Shutdown();
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