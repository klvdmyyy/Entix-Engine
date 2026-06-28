#include "GUI/Context.h"
#include "GUI/Window.h"

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

#include <SDL3/SDL.h>

namespace ERUNTIME_NAMESPACE::GUI {
    Context::Context(const Ref<::ERUNTIME_NAMESPACE::Context>& rendererContext)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        io.IniFilename = nullptr;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();

        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        rendererContext->InitGUI();
    }

    Context::~Context()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void Context::OnPreUpdate()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        
        for(auto it = m_windows.begin(); it != m_windows.end(); it++) {
            ImGui::SetNextWindowPos(ImVec2((*it)->GetStartXPos(), (*it)->GetStartYPos()), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2((*it)->GetStartWidth(), (*it)->GetStartHeight()), ImGuiCond_FirstUseEver);
            (*it)->BeginRender();
        }
    }

    void Context::OnPostRender()
    {
        for(auto it = m_windows.rbegin(); it != m_windows.rend(); it++) {
            (*it)->EndRender();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}