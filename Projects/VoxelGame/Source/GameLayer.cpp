#include "GameLayer.h"

#include "PlayerController.h"

GameLayer::GameLayer()
    : Layer("GameLayer")
{
}

void GameLayer::OnAttach()
{
    Scene& scene = Application::Get().GetCurrentScene();

    Entity player = scene.CreateEntity("Player");

    NativeScriptComponent& playerScript = player.AddComponent<NativeScriptComponent>();
    playerScript.Bind<PlayerController>();

    Entity camera = scene.CreateEntity("PlayerCamera");

    // Set camera child of player
    camera.AddOrReplaceComponent<TransformComponent>(player);
}

void GameLayer::OnTick(float deltaTime)
{
    Application::Get().GetCurrentScene().OnTick(deltaTime);
}

void GameLayer::OnRender()
{
    Application::Get().GetCurrentScene().OnRender();
}