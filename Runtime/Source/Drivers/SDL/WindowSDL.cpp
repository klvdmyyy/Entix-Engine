#include "Drivers/SDL/WindowSDL.h"

#include "Core/Log.h"
#include "Core/EventBus.h"

using Core::WindowSpecification;
using Core::Ref;
using Core::EventBus;
using Core::WindowCloseEvent;

Ref<Core::Window> Core::Window::Create(const WindowSpecification& spec)
{
	return Ref<Window>(new Drivers::WindowSDL(spec));
}

namespace Drivers
{
	WindowSDL::WindowSDL(const WindowSpecification& spec)
		: m_Spec(spec)
	{
		Init();
	}

	WindowSDL::~WindowSDL()
	{
		Shutdown();
	}

	void WindowSDL::Init()
	{
		m_Window = SDL_CreateWindow(m_Spec.Title.c_str(), m_Spec.Width, m_Spec.Height, 0);
	}

	void WindowSDL::Shutdown()
	{
		SDL_DestroyWindow(m_Window);
	}

	uint32_t WindowSDL::GetWidth() const
	{
		return 0;
	}

	uint32_t WindowSDL::GetHeight() const
	{
		return 0;
	}

	void WindowSDL::OnUpdate()
	{
		/* TODO: If we need more than one window we should move
		event handling to Core::Application class */
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			{
				WindowCloseEvent e(event.window.windowID);
				EventBus::Get().PublishEvent(e);
				break;
			}
			default:
				break;
			}
		}
	}
}