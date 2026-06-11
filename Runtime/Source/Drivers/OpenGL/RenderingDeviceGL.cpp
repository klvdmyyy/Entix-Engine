#include "Drivers/OpenGL/RenderingDeviceGL.h"

#include "Core/Log.h"

#include <glad/glad.h>
#include <SDL3/SDL.h>

namespace Drivers
{
	RenderingDeviceGL::RenderingDeviceGL(size_t index)
		: m_Index(index)
	{
		E_ASSERT(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress), "Failed to load GL loader function.");

		if constexpr (ENABLE_VALIDATION_LAYERS)
		{
			E_INFO("OpenGL Debug Messaging is enabled!");
			/* TODO: Debug messenger initialization here */
		}
	}

	RenderingDeviceGL::~RenderingDeviceGL()
	{
	}

	RHI::RenderingContext* RenderingDeviceGL::CreateContext(Core::Window& window)
	{
		return new RenderingContextGL(window);
	}
}