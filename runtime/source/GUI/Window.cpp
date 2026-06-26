#include "GUI/Window.h"

#include <imgui.h>

namespace ERUNTIME_NAMESPACE::GUI {
    Window::Window(const String& name)
        : k_name(name), m_show(false), m_isOpen(false)
    {
    }

    void Window::Show()
    {
        m_show = true;
    }

    void Window::Hide()
    {
        m_show = false;
    }

    void Window::BeginRender()
    {
        if(m_show) {
            m_isOpen = m_show;
            ImGui::Begin(GetName().c_str(), &m_isOpen);
        }
    }

    void Window::EndRender()
    {
        if(m_show) {
            this->Draw();
            ImGui::End();
            m_show = m_isOpen;
        }
    }
}