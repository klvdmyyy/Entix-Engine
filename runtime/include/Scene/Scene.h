// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/Types.h"
#include "Core/String.h"
#include "Core/Memory.h"

#include "Renderer/Context.h"

#include <unordered_map>

#include <entt/entt.hpp>

namespace ERUNTIME_NAMESPACE {
    class Entity;

    class Scene {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const String& name = String());
        Entity CreateEntityWithUUID(UUID uuid, const String& name = String());
        void DestroyEntity(Entity entity);

        Entity FindEntityByName(StringView name);
        Entity GetEntityByUUID(UUID uuid);

        void OnTick(float deltaTime);

    private:
        friend class Entity;

        std::unordered_map<UUID, entt::entity> m_entityMap{};
        entt::registry m_registry{};

        Ref<Renderer::Context> m_rendererContext;
    };
}