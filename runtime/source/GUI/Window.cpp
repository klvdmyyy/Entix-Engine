#include "GUI/Window.h"

#include <imgui.h>

namespace ERUNTIME_NAMESPACE::GUI {
    Window::Window(const String& name, Uint32 startWidth, Uint32 startHeight, Uint32 startXPos, Uint32 startYPos)
        : k_name(name), m_show(false), m_isOpen(false),
          k_startWidth(startWidth), k_startHeight(startHeight),
          k_startXPos(startXPos), k_startYPos(startYPos)
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