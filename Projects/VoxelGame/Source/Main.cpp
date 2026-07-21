#define GAME_MAIN
#include <GameFramework.h>

#include "GameLayer.h"

static const ApplicationSpecification g_spec = {
    .name = "Voxel Game",
    .description = "Minecraft-like game built with Entix Engine",
    .windowSpec = {
        .width = 800,
        .height = 600,
        .title = "VoxelGame",
        .resizable = true,
    },
};

class VoxelGame : public Application {
public:
    VoxelGame() : Application(g_spec)
    {
        PushLayer<GameLayer>();
    }
};

Application* CreateApplication()
{
    return new VoxelGame();
}