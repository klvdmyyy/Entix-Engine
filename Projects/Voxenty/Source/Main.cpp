#define GAME_MAIN
#include <GameFramework.h>

#include "SquareMesh.h"

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
        ResourceManager::Instance().LoadShader(EX_GET_SHADER("SimpleShader.glsl"));
        
        auto& scene = GetCurrentScene();

        Entity square = scene.CreateEntity("Square");

        square.AddComponent<StaticMeshComponent>(CreateSquareMesh(GetRendererContext()));

        Entity camera = scene.CreateEntity("Camera");

        camera.AddComponent<CameraComponent>();

        TransformComponent& cameraTransform = camera.GetComponent<TransformComponent>();
        cameraTransform.position = Float3(0.0f, 0.0f, 2.0f);
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