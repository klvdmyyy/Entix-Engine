#include "GameLayer.h"

#include "SquareMesh.h"

#include "Scripts/Player.h"

#include <GameFramework/ShaderLoader.h>

#include <Resources/ResourceManager.h>

#include <Input/ActionSystem.h>

GameLayer::GameLayer()
    : Layer("GameLayer")
{
}

void GameLayer::OnAttach()
{
    auto& rm = ResourceManager::Instance();

    rm.RegisterLoader<ShaderLoader>(Application::Get().GetRendererContext());

    ActionSystem::Instance().SetActionMap(ActionMap::LoadFromFile("C:\\Users\\User\\Desktop\\Entix-Engine\\Projects\\Editor\\action_map.json"));
    ActionSystem::Instance().PushContext(ActionContext{"MoveForward", "MoveBackward", "MoveLeft", "MoveRight", "Menu"});
    
    Scene& scene = Application::Get().GetCurrentScene();

    Entity player = scene.CreateEntity("Player");
    player.AddComponent<NativeScriptComponent>().Bind<Player>();
    player.GetComponent<TransformComponent>().position.z = 2.0f;

    Entity playerCamera = scene.CreateEntity("Camera");
    playerCamera.AddOrReplaceComponent<TransformComponent>(player);
    playerCamera.AddComponent<CameraComponent>();
    
    Entity square = scene.CreateEntity("Square");
    StaticMeshComponent& mesh = square.AddComponent<StaticMeshComponent>(CreateSquareMesh(Application::Get().GetRendererContext()));

    mesh.material.shader = rm.Load<Renderer::Shader, ShaderLoader>(EX_GET_SHADER("SimpleShader.glsl"));
}

void GameLayer::OnTick(Timestep deltaTime)
{
    Application::Get().GetCurrentScene().OnTick(deltaTime);
}

void GameLayer::OnRender()
{
    Application::Get().GetCurrentScene().OnRender();
}