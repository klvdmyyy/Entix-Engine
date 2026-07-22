#define GAME_MAIN
#include <GameFramework.h>

#include "EditorLayer.h"

#include <imgui.h>

#include <tracy/Tracy.hpp>

static const ApplicationSpecification g_spec = {
    .name = "Entix Editor",
    .description = "Editor for Entix-Engine",
    .windowSpec = {
        .width = 1280,
        .height = 720,
        .title = "Entix Engine 1.0",
        .resizable = true,
        .maximized = true,
    },
};

class EntixEditor : public Application
{
public:
    EntixEditor() : Application(g_spec)
    {
        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(PushOverlay<GUILayer>()->GetImGuiContext()));
        PushLayer<EditorLayer>();
    }
};

Application* CreateApplication()
{
    return new EntixEditor();
}