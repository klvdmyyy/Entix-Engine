#include "GameFramework/GUILayer.h"

#include "GameFramework/Application.h"

#include <imgui.h>

GUILayer::GUILayer()
    : Layer("GUILayer")
{
}

void GUILayer::OnAttach()
{
    m_rendererContext = Application::Get().GetRendererContext();

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

    m_rendererContext->InitGUI();
}

void GUILayer::OnDetach()
{
    m_rendererContext->ShutdownGUI();
    ImGui::DestroyContext();
}

void GUILayer::OnPreRender()
{
    m_rendererContext->NewGUIFrame();
    ImGui::NewFrame();
}

void GUILayer::OnPostRender()
{
    ImGui::Render();
    m_rendererContext->RenderGUI();
}