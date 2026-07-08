#include "Drivers/SDL3/WindowSDL.h"

#include "Core/Assert.h"
#include "Core/EventSystem.h"

#include "WSI/Input.h"

#include <imgui.h>
#include <imgui_impl_sdl3.h>

Window* Window::Create(const WindowSpecification& spec)
{
    return new WindowSDL(spec);
}

WindowSDL::WindowSDL(const WindowSpecification& spec)
{
    EX_ASSERT(SDL_Init(SDL_INIT_VIDEO), "failed to initialize sdl. sdl error: {}", SDL_GetError());

    // CONFIGURE OPENGL ATTRIBUTES USING SDL:
    int context_flags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
#ifdef _DEBUG
    context_flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    Uint32 flags = SDL_WINDOW_OPENGL;
        
    if(spec.resizable)
        flags |= SDL_WINDOW_RESIZABLE;

    m_window = SDL_CreateWindow(spec.title.c_str(), spec.width, spec.height, flags);

    EX_ASSERT(m_window, "failed to create sdl window. sdl error: {}", SDL_GetError());

    SDL_SetWindowFullscreen(m_window, spec.fullscreen);
}

WindowSDL::~WindowSDL()
{
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void WindowSDL::Update()
{
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if(m_guiEnabled)
            ImGui_ImplSDL3_ProcessEvent(&event);
        
        switch(event.type) {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
            WindowCloseEvent e(event.window.windowID);
            EventBus::Instance().PublishEvent(e);
            break;
        }
        case SDL_EVENT_MOUSE_MOTION: {
            MouseMotionEvent e(event.motion.x, event.motion.y);
            EventBus::Instance().PublishEvent(e);
            break;
        }
        default:
            break;
        }
    }
}

void WindowSDL::EnableGUIUpdate()
{
    m_guiEnabled = true;
}