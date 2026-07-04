#define GAME_MAIN
#include <GameFramework.h>

#include <Core/Debug/Log.h>

static const ApplicationSpecification g_spec = {
    .name = "Voxenty",
    .description = "Simple Minecraft-like Game",
    .windowSpec = {
        .width = 800,
        .height = 600,
        .title = "Voxenty 1.0",
        .resizable = false,
    },
};

class Voxenty : public Application
{
public:
    Voxenty() : Application(g_spec)
    {
    }

    void OnInit() final
    {
        EX_LOG(Warning, LogCategory::IO, "Hello, {}!", "World");
    }

    void OnTick() final
    {
    }

    void OnShutdown() final
    {
    }
};

Application* CreateApplication()
{
    return new Voxenty();
}