#include "GameLayer.h"

#include "SquareMesh.h"

#include "Scripts/Player.h"
#include "Scripts/PlayerCamera.h"

#include <WSI/ActionSystem.h>

GameLayer::GameLayer()
    : Layer("GameLayer")
{
}

void GameLayer::OnAttach()
{
    ResourceManager::Instance().LoadShader(EX_GET_SHADER("SimpleShader.glsl"));
    ActionSystem::Instance().SetActionMap(ActionMap::LoadFromFile("../Projects/Editor/action_map.json"));
    ActionSystem::Instance().PushContext(ActionContext{"MoveForward", "MoveBackward", "MoveLeft", "MoveRight", "Menu"});
    
    Scene& scene = Application::Get().GetCurrentScene();

    Entity player = scene.CreateEntity("Player");
    player.AddComponent<NativeScriptComponent>().Bind<Player>();
    player.GetComponent<TransformComponent>().position.z = 2.0f;

    Entity playerCamera = scene.CreateEntity("Camera");
    playerCamera.AddOrReplaceComponent<TransformComponent>(player);
    playerCamera.AddComponent<CameraComponent>();
    playerCamera.AddComponent<NativeScriptComponent>().Bind<PlayerCamera>();
    
    Entity square = scene.CreateEntity("Square");
    square.AddComponent<StaticMeshComponent>(CreateSquareMesh(Application::Get().GetRendererContext()));
}

void GameLayer::OnTick(Timestep deltaTime)
{
    Application::Get().GetCurrentScene().OnTick(deltaTime);
}

void GameLayer::OnRender()
{
    Application::Get().GetCurrentScene().OnRender();
}