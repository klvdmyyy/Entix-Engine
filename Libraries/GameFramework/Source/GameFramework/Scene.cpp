#include "GameFramework/Scene.h"

#include "GameFramework/Entity.h"

#include "Core/Resources/ResourceManager.h"
#include "Core/Debug/Log.h"

Scene::Scene(const Ref<Renderer::Context>& rendererContext)
    : m_rendererContext(rendererContext)
{
}

Scene::~Scene()
{
}

Entity Scene::CreateEntity(const String& name)
{
    return CreateEntityWithUUID(UUID(), name);
}

Entity Scene::CreateEntityWithUUID(UUID uuid, const String& name)
{
    Entity entity = { m_registry.create(), this };
    entity.AddComponent<IDComponent>(uuid);
    entity.AddComponent<TransformComponent>();
    auto& tc = entity.AddComponent<TagComponent>();
    tc.tag = name.empty() ? "Entity" : name;

    m_entityMap[uuid] = entity;

    return entity;
}

Entity Scene::FindEntityByName(StringView name)
{
    auto view = m_registry.view<TagComponent>();
    for(auto entity : view) {
        const TagComponent& tc = view.get<TagComponent>(entity);
        if(tc.tag == name)
            return Entity{ entity, this };
    }
    return Entity{};
}

Entity Scene::GetEntityByUUID(UUID uuid) {
    auto it = m_entityMap.find(uuid);
    if(it != m_entityMap.end())
        return Entity{ it->second, this };

    return Entity{};
}

void Scene::OnTick(float deltaTime)
{
    // Update camera
    {
        auto view = m_registry.view<TransformComponent, CameraComponent>();
        for(auto entity : view) {
            auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

            camera.Update(transform, 800.0f / 600.0f);
        }
    }

    // Render Mesh
    m_rendererContext->SetClearColor(0.2f, 0.2f, 0.2f);
    m_rendererContext->Clear();

    m_rendererContext->BeginScene();

    {
        auto camerasView = m_registry.view<TransformComponent, CameraComponent>();
        auto group = m_registry.group<TransformComponent>(entt::get<StaticMeshComponent>);
            
        for(auto cameraEntity : camerasView) {
            auto camera = camerasView.get<CameraComponent>(cameraEntity);
                
            for(auto entity : group) {
                auto [transform, mesh] = group.get<TransformComponent, StaticMeshComponent>(entity);

                auto shader = ResourceManager::Instance().GetShader(mesh.material.shader);
                    
                shader->Bind();
                shader->SetFloat4x4("model", transform.GetLocalMatrix());
                shader->SetFloat4x4("view", camera.GetView());
                shader->SetFloat4x4("projection", camera.GetProjection());
                
                m_rendererContext->Submit(shader, mesh.vertexArray);
            }
        }
    }

    m_rendererContext->EndScene();
}