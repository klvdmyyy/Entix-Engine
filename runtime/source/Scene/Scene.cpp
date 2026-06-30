#include "Scene/Scene.h"

#include "Scene/Entity.h"

namespace ERUNTIME_NAMESPACE {
    Scene::Scene()
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
    }
}