#include "GameFramework/Scene.h"

#include "GameFramework/Entity.h"

#include "GameFramework/ScriptableEntity.h"

#include "Core/Resources/ResourceManager.h"
#include "Core/Debug/Log.h"

#include <unordered_map>
#include <functional>
#include <vector>

Scene::Scene(const Ref<Renderer::Context>& rendererContext)
    : m_rendererContext(rendererContext)
{
}

Scene::~Scene()
{
}

Entity Scene::CreateEntity(const String& name)
{
    return CreateEntityWithUuid(Uuid(), name);
}

Entity Scene::CreateEntityWithUuid(Uuid uuid, const String& name)
{
    Entity entity = { m_registry.create(), this };
    entity.AddComponent<IdComponent>(uuid);
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

Entity Scene::GetEntityByUuid(Uuid uuid) {
    auto it = m_entityMap.find(uuid);
    if(it != m_entityMap.end())
        return Entity{ it->second, this };

    return Entity{};
}

void Scene::OnTick(Timestep deltaTime)
{   
    // Update transformations
    {
        auto view = m_registry.view<TransformComponent>();

        std::unordered_map<entt::entity, entt::entity> parentMap;
        std::vector<entt::entity> roots;

        for(auto entity : view) {
            auto& transform = view.get<TransformComponent>(entity);
            transform.UpdateLocalMatrix();

            if(transform.parent != entt::null) {
                parentMap[entity] = transform.parent;
            } else {
                roots.push_back(entity);
            }
        }

        std::unordered_map<entt::entity, std::vector<entt::entity>> childrenMap;
        for(auto [child, parent] : parentMap) {
            childrenMap[parent].push_back(child);
        }

        std::function<void(entt::entity, const Float4x4&)> updateWorld =
            [&](entt::entity entity, const Float4x4& parentWorld)
            {
                auto& transform = view.get<TransformComponent>(entity);
                transform.UpdateWorldMatrix(parentWorld);

                auto it = childrenMap.find(entity);
                if(it != childrenMap.end()) {
                    const auto& worldMatrix = transform.GetWorldMatrix();
                    for(auto child : it->second) {
                        updateWorld(child, worldMatrix);
                    }
                }
            };

        for(auto root : roots) {
            updateWorld(root, Float4x4(1.0f));
        }
    }

    // Update camera
    {
        auto view = m_registry.view<TransformComponent, CameraComponent>();
        for(auto entity : view) {
            auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

            camera.Update(transform, 800.0f / 600.0f);
        }
    }

    // Update scripts
    {
        auto view = m_registry.view<NativeScriptComponent>();
        for(auto entity : view) {
            auto& nsc = view.get<NativeScriptComponent>(entity);

            if(!nsc.instance) {
                nsc.instance = nsc.InstantiateScript();
                nsc.instance->m_entity = Entity{ entity, this };
                nsc.instance->OnCreate();
            }

            nsc.instance->OnTick(deltaTime);
        }
    }
}

void Scene::OnRender()
{
    m_rendererContext->SetClearColor(0.2f, 0.2f, 0.2f);
    m_rendererContext->Clear();

    m_rendererContext->BeginScene();

    // Render Static Mesh
    {
        auto camerasView = m_registry.view<TransformComponent, CameraComponent>();
        auto group = m_registry.group<TransformComponent>(entt::get<StaticMeshComponent>);
            
        for(auto cameraEntity : camerasView) {
            auto camera = camerasView.get<CameraComponent>(cameraEntity);
                
            for(auto entity : group) {
                auto [transform, mesh] = group.get<TransformComponent, StaticMeshComponent>(entity);

                auto shader = ResourceManager::Instance().GetShader(mesh.material.shader);
                    
                shader->Bind();
                shader->SetFloat4x4("model", transform.GetWorldMatrix());
                shader->SetFloat4x4("view", camera.GetView());
                shader->SetFloat4x4("projection", camera.GetProjection());
                
                m_rendererContext->Submit(shader, mesh.vertexArray);
            }
        }
    }

    m_rendererContext->EndScene();
}