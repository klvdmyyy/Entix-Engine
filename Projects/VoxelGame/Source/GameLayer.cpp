#include "GameLayer.h"

#include "Scripts/Player.h"

#include <GameFramework/ShaderLoader.h>
#include <GameFramework/ObjMeshLoader.h>
#include <GameFramework/TextureLoader.h>

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
    rm.RegisterLoader<ObjMeshLoader>(Application::Get().GetRendererContext());
    rm.RegisterLoader<TextureLoader>(Application::Get().GetRendererContext());

    ActionSystem::Instance().SetActionMap(ActionMap::LoadFromFile("C:\\Users\\User\\Desktop\\Entix-Engine\\Projects\\Editor\\action_map.json"));
    ActionSystem::Instance().PushContext(ActionContext{"MoveForward", "MoveBackward", "MoveLeft", "MoveRight", "Menu"});
    
    Scene& scene = Application::Get().GetCurrentScene();

    Entity player = scene.CreateEntity("Player");
    player.AddComponent<NativeScriptComponent>().Bind<Player>();
    player.GetComponent<TransformComponent>().position.z = 2.0f;

    Entity playerCamera = scene.CreateEntity("Camera");
    playerCamera.AddOrReplaceComponent<TransformComponent>(player);
    playerCamera.AddComponent<CameraComponent>();
    
    Entity cube = scene.CreateEntity("Cube");
    StaticMeshComponent& mesh = cube.AddComponent<StaticMeshComponent>();

    mesh.vertexArray = rm.Load<Renderer::VertexArray, ObjMeshLoader>(EX_GET_MODEL("Cube.obj"));
    mesh.material.shader = rm.Load<Renderer::Shader, ShaderLoader>(EX_GET_SHADER("Textures.glsl"));
    mesh.material.texture = rm.Load<Renderer::Texture, TextureLoader>(EX_GET_TEXTURE("Wall.jpg"));

    rm.LoadAsync<Renderer::Shader, ShaderLoader>("SOME_SORT_OF_SHADER!");
}

void GameLayer::OnTick(Timestep deltaTime)
{
    Application::Get().GetCurrentScene().OnTick(deltaTime);
}

void GameLayer::OnRender()
{
    Application::Get().GetCurrentScene().OnRender();
}