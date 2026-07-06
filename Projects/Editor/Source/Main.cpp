#define GAME_MAIN
#include <GameFramework.h>

#include "EditorLayer.h"

static const ApplicationSpecification g_spec = {
    .name = "Entix Editor",
    .description = "Editor for Entix-Engine",
    .windowSpec = {
        .width = 800,
        .height = 600,
        .title = "Entix Engine 1.0",
        .resizable = false,
    },
};

class EntixEditor : public Application
{
public:
    EntixEditor() : Application(g_spec)
    {
        PushOverlay<GUILayer>();
        PushLayer<EditorLayer>();
    }
};

Application* CreateApplication()
{
    return new EntixEditor();
}