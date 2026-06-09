#include "Core/Window.h"

using namespace Core;

class WindowImpl_SDL;

Ref<Window> Window::Create(const WindowSpecification& spec)
{
	return CreateRef<WindowImpl_SDL>(spec);
}

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

private:
	void Init()
	{
	}

	void Shutdown()
	{
	}

	const WindowSpecification m_Spec;
};

