#include "GameLayer.h"

#include "SquareMesh.h"

#include "Scripts/Player.h"

GameLayer::GameLayer()
    : Layer("GameLayer")
{
}

void GameLayer::OnAttach()
{
    ResourceManager::Instance().LoadShader(EX_GET_SHADER("SimpleShader.glsl"));
    
    Scene& scene = Application::Get().GetCurrentScene();

    Entity player = scene.CreateEntity("Player");
    player.AddComponent<NativeScriptComponent>().Bind<Player>();
    
    Entity playerMesh = scene.CreateEntity("PlayerMesh");
    playerMesh.AddOrReplaceComponent<TransformComponent>(player); // Make PlayerMesh child of Player
    playerMesh.AddComponent<StaticMeshComponent>(CreateSquareMesh(Application::Get().GetRendererContext()));

    Entity camera = scene.CreateEntity("Camera");

    camera.AddComponent<CameraComponent>();

    // camera.AddOrReplaceComponent<TransformComponent>(&player);
    camera.GetComponent<TransformComponent>().position.z = 2.0f;
}

void GameLayer::OnTick(Timestep deltaTime)
{
    Application::Get().GetCurrentScene().OnTick(deltaTime);
}

void GameLayer::OnRender()
{
    Application::Get().GetCurrentScene().OnRender();
}