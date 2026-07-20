#include "GameFramework/GUILayer.h"

#include "GameFramework/Application.h"

#include "Core/PlatformDetection.h"

#include "Core/Debug/Log.h"
#include "Core/Debug/LogFormatters.h"

#include <imgui.h>

#include <tracy/Tracy.hpp>

const LogCategory GUILayer::sk_logCategory{"GUI", DefaultFormatter::Instance()};

GUILayer::GUILayer()
    : Layer("GUILayer")
{
}

void GUILayer::OnAttach()
{
    ZoneScopedN("Initializing GUI Layer");

    EX_LOG(Trace, sk_logCategory, "Initializing GUI layer");

    m_rendererContext = Application::Get().GetRendererContext();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

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
    ZoneScopedN("Destroying GUI Layer");

    EX_LOG(Trace, sk_logCategory, "Destroying GUI layer");
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

#ifdef PLATFORM_WINDOWS
    if(ImGuiIO io = ImGui::GetIO(); io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        m_rendererContext->Restore();
    }
#endif
}