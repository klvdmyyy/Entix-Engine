#include "EditorLayer.h"

#include <imgui.h>

EditorLayer::EditorLayer()
    : Layer("EditorLayer")
{
}

void EditorLayer::OnRender()
{
    bool show = true;
    ImGui::ShowDemoWindow(&show);
}