// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/Types.h"
#include "Core/String.h"
#include "Core/Memory.h"

#include "Core/Timestep.h"

#include "Renderer/Context.h"

#include "GameFramework/Components.h"

#include <unordered_map>

#include <entt/entt.hpp>

class Entity;

class Scene {
public:
    Scene(const Ref<Renderer::Context>& rendererContext);
    ~Scene();

    Entity CreateEntity(const String& name = String());
    Entity CreateEntityWithUuid(Uuid uuid, const String& name = String());
    void DestroyEntity(Entity entity);

    Entity FindEntityByName(StringView name);
    Entity GetEntityByUuid(Uuid uuid);

    void OnTick(Timestep deltaTime);
    void OnRender(CameraComponent* forcedCamera = nullptr);

    void EachEntity(std::function<void(Entity)> callback) noexcept;

    template<typename... Components>
    FORCE_INLINE
    inline auto GetAllEntitiesWith()
    {
        return m_registry.view<Components...>();
    }

private:
    friend class Entity;

    std::unordered_map<Uuid, entt::entity> m_entityMap{};
    entt::registry m_registry{};

    Ref<Renderer::Context> m_rendererContext;
};