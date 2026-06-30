#include "Scene/Scene.h"

#include "Scene/Entity.h"

#include "Core/Resources/ResourceManager.h"

namespace ERUNTIME_NAMESPACE {
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
        m_rendererContext->BeginScene();

        m_rendererContext->SetClearColor(0.2f, 0.2f, 0.2f);
        m_rendererContext->Clear();

        {
            auto group = m_registry.group<TransformComponent>(entt::get<StaticMeshComponent>);
            for(auto entity : group) {
                auto [transform, mesh] = group.get<TransformComponent, StaticMeshComponent>(entity);
                
                m_rendererContext->Submit(ResourceManager::Instance().GetShader(mesh.material.shader), mesh.vertexArray);
            }
        }

        // {
        //     auto group = m_registry.group<TransformComponent>(entt::get<StaticMeshComponent);
        //     auto camerasView = m_registry.view<TransformComponent, CameraComponent>();

        //     for(auto [cameraTransform, camera] : camerasView.each()) {
        //         for(auto [transform, mesh] : group.each()) {
        //             m_rendererContext->Submit(ResourceManager::Instance().GetShader(mesh.material.shader), mesh.vertexArray);
        //         }
        //     }
        // }

        m_rendererContext->EndScene();
    }
}