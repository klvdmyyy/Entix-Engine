#pragma once

#include "ERuntimeExport.h"

#include "Core/Memory.h"
#include "Core/Window.h"

#include <SDL3/SDL.h>

namespace Drivers
{
	class WindowSDL : public Core::Window
	{
	public:
		WindowSDL(const Core::WindowSpecification& spec);
		~WindowSDL();

		uint32_t GetWidth() const final;
		uint32_t GetHeight() const final;

		void OnUpdate() final;

	private:
		void Init();
		void Shutdown();

		const Core::WindowSpecification m_Spec;

		SDL_Window* m_Window;
	};

	
}