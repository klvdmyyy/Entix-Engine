#include "GameFramework/Scene.h"

#include "GameFramework/Entity.h"

#include "GameFramework/ScriptableEntity.h"

#include "Core/Debug/Log.h"

#include "Resources/ResourceManager.h"

#include "GameFramework/Application.h"

#include <unordered_map>
#include <functional>
#include <vector>

#include <tracy/Tracy.hpp>

Scene::Scene(const Ref<Renderer::Context>& rendererContext)
    : m_rendererContext(rendererContext)
{
}

Scene::~Scene()
{
}

Entity Scene::CreateEntity(const String& name)
{
    ZoneScoped;

    return CreateEntityWithUuid(Uuid(), name);
}

Entity Scene::CreateEntityWithUuid(Uuid uuid, const String& name)
{
    ZoneScoped;

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
    ZoneScoped;

    auto view = m_registry.view<TagComponent>();
    for(auto entity : view) {
        const TagComponent& tc = view.get<TagComponent>(entity);
        if(tc.tag == name)
            return Entity{ entity, this };
    }
    return Entity{};
}

Entity Scene::GetEntityByUuid(Uuid uuid) {
    ZoneScoped;

    auto it = m_entityMap.find(uuid);
    if(it != m_entityMap.end())
        return Entity{ it->second, this };

    return Entity{};
}

static constexpr const char* TICK_FRAME = "Scene OnTick";

void Scene::OnTick(Timestep deltaTime)
{   
    FrameMarkStart(TICK_FRAME);

    constexpr const char* TRANSFORM_FRAME = "Update transformations";
    // Update transformations
    {
        FrameMarkStart(TRANSFORM_FRAME);
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
        FrameMarkEnd(TRANSFORM_FRAME);
    }

    constexpr const char* CAMERA_FRAME = "Update camera";

    // Update camera
    {
        FrameMarkStart(CAMERA_FRAME);
        auto view = m_registry.view<TransformComponent, CameraComponent>();
        for(auto entity : view) {
            auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

            Renderer::Framebuffer* fb = camera.framebuffer ? camera.framebuffer.get() : nullptr;
            Renderer::Rect targetRect;

            if(fb)
            {
                auto& spec = fb->GetSpecification();
                targetRect = Renderer::Rect(spec.width, spec.height);
            }
            else
            {
                auto window = Application::Get().GetWindow();
                targetRect = Renderer::Rect(window->GetWidth(), window->GetHeight());
            }

            camera.viewport.UpdateAbsolute(targetRect);

            camera.Update(transform);
        }
        FrameMarkEnd(CAMERA_FRAME);
    }

    constexpr const char* NATIVE_SCRIPT_FRAME = "Updating native scripts";

    // Update scripts
    {
        FrameMarkStart(NATIVE_SCRIPT_FRAME);
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
        FrameMarkEnd(NATIVE_SCRIPT_FRAME);
    }

    FrameMarkEnd(TICK_FRAME);
}

static constexpr const char* RENDERING_FRAME = "Scene OnRender";

void Scene::OnRender()
{
    FrameMarkStart(RENDERING_FRAME);

    m_rendererContext->SetClearColor(0.2f, 0.2f, 0.2f);
    m_rendererContext->Clear();

    constexpr const char* STATIC_MESH_FRAME = "StaticMesh rendering";

    {
        auto camerasView = m_registry.view<TransformComponent, CameraComponent>();
        auto group = m_registry.group<TransformComponent>(entt::get<StaticMeshComponent>);
            
        for(auto cameraEntity : camerasView) {
            auto& camera = camerasView.get<CameraComponent>(cameraEntity);

            Renderer::Framebuffer* fb = camera.framebuffer ? camera.framebuffer.get() : nullptr;

            m_rendererContext->BeginScene(camera.viewport);

            if(fb) fb->Bind();
            
            m_rendererContext->SetClearColor(0.2f, 0.2f, 0.2f);
            m_rendererContext->Clear();

            // Render Static Mesh
            for(auto entity : group) {
                FrameMarkStart(STATIC_MESH_FRAME);
                auto [transform, mesh] = group.get<TransformComponent, StaticMeshComponent>(entity);

                auto& shader = mesh.material.shader;
                auto& texture = mesh.material.texture;
                EX_DEBUG_ASSERT(shader.IsValid(), "Usage of invalid shader in material!");
                
                shader->Bind();
                
                shader->SetFloat4x4("model", transform.GetWorldMatrix());
                shader->SetFloat4x4("view", camera.GetView());
                shader->SetFloat4x4("projection", camera.GetProjection());

                if(texture) {
                    texture->Bind();
                    shader->SetInt("texture", 0);
                }
                
                m_rendererContext->Submit(shader.Get(), mesh.vertexArray.Get());
                FrameMarkEnd(STATIC_MESH_FRAME);
            }

            if(fb) fb->Unbind();

            m_rendererContext->EndScene();
        }
    }

    FrameMarkEnd(RENDERING_FRAME);
}