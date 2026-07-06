#include "EditorLayer.h"

#include <imgui.h>

EditorLayer::EditorLayer()
    : Layer("EditorLayer")
{
}

void EditorLayer::OnRender()
{
    // Render the scene
    Application::Get().GetCurrentScene().OnRender();

    bool show = true;
    ImGui::ShowDemoWindow(&show);
}