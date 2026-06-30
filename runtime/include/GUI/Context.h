// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/Memory.h"

#include "Renderer/Context.h"

#include "GUI/Window.h"

#include <type_traits>

namespace ERUNTIME_NAMESPACE::GUI {
    class Context {
    public:
        Context(const Ref<Renderer::Context>& rendererContext);
        ~Context();
        
        template<typename T, typename... Args>
        T& CreateWindow(Args&&... args)
        {
            static_assert(std::is_base_of<Window, T>::value, "Window must be derived from IGUI::Window class");

            auto window = Scope<Window>(new T(std::forward<Args>(args)...));
            T& windowRef = dynamic_cast<T&>(*window);
            m_windows.push_back(std::move(window));
            return windowRef;
        }

        void OnPreUpdate();
        void OnPostRender();

    private:
        std::vector<Scope<Window>> m_windows;
    };
}