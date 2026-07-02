#define GAME_MAIN
#include <GameFramework.h>

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
    }
};

Application* CreateApplication()
{
    return new Voxenty();
}