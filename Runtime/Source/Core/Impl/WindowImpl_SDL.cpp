#include "Core/Window.h"

#include <SDL3/SDL.h>

using namespace Core;

class WindowImpl_SDL : public Window
{
public:
	WindowImpl_SDL(const WindowSpecification& spec)
		: m_Spec(spec)
	{
		Init();
	}

	~WindowImpl_SDL()
	{
		Shutdown();
	}

	uint32_t GetWidth() const final
	{
		return 0;
	}

	uint32_t GetHeight() const final
	{
		return 0;
	}

	void OnUpdate() final
	{
		/* TODO: If we need more than one window we should move
		event handling to Core::Application class */
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
		}
	}

private:
	void Init()
	{
		m_Window = SDL_CreateWindow(m_Spec.Title.c_str(), m_Spec.Width, m_Spec.Height, 0);
	}

	void Shutdown()
	{
		SDL_DestroyWindow(m_Window);
	}

	const WindowSpecification m_Spec;

	SDL_Window* m_Window;
};

Ref<Window> Window::Create(const WindowSpecification& spec)
{
	return Ref<Window>(new WindowImpl_SDL(spec));
}