#include "Platform/SDL/WindowSDL.h"

#include <SDL3/SDL_vulkan.h>

namespace Entix
{
    WindowSDL::WindowSDL(const WindowConfig& config)
        : k_config(config)
    {
        Initialize().Unwrap();
    }

    WindowSDL::~WindowSDL()
    {
        Shutdown().Unwrap();
    }

    Result<void> WindowSDL::Initialize()
    {
        m_window = SDL_CreateWindow(k_config.title.c_str(), k_config.width, k_config.height, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_VULKAN);

        if(m_window == nullptr)
            return Error(SDL_GetError());
        
        return {};
    }

    Result<void> WindowSDL::Shutdown()
    {
        SDL_DestroyWindow(m_window);

        return {};
    }

    Result<void*> WindowSDL::GetWindowHandle() const noexcept
    {
        if(m_window == nullptr)
            return Error("SDL_Window is nullptr!");

        return m_window;
    }

    Result<void*> WindowSDL::CreateVulkanSurface(void* instance) noexcept
    {
        VkSurfaceKHR surface;
        if(!SDL_Vulkan_CreateSurface(m_window, static_cast<VkInstance_T*>(instance), nullptr, &surface))
        {
            return Error(SDL_GetError());
        }

        return static_cast<void*>(surface);
    }

    Result<Uint32> WindowSDL::GetWidth() const noexcept
    {
        if(m_window == nullptr)
            return Error("SDL_Window is nullptr!");

        Int32 w, h;
        SDL_GetWindowSize(m_window, &w, &h);

        return static_cast<Uint32>(w);
    }

    Result<Uint32> WindowSDL::GetHeight() const noexcept
    {
        if(m_window == nullptr)
            return Error("SDL_Window is nullptr!");

        Int32 w, h;
        SDL_GetWindowSize(m_window, &w, &h);

        return static_cast<Uint32>(h);
    }

    Result<void> WindowSDL::GrabCursor(bool value) noexcept
    {
        if(m_window == nullptr)
            return Error("SDL_Window is nullptr!");

        if(!SDL_SetWindowRelativeMouseMode(m_window, value))
            return Error(SDL_GetError());
        
        return {};
    }

    Result<void> WindowSDL::ToggleGrabCursor() noexcept
    {
        if(m_window == nullptr)
            return Error("SDL_Window is nullptr!");
        
        EX_TRY(GrabCursor(!SDL_GetWindowRelativeMouseMode(m_window)));

        return {};
    }
}