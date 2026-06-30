// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/TypeName.h"
#include "Core/Assert.h"
#include "Core/Types.h"

#include "Scene/Scene.h"
#include "Scene/Components.h"

#include <entt/entt.hpp>

namespace ERUNTIME_NAMESPACE {
    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;
        
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            EX_ASSERT(!HasComponent<T>(), "Entity already has component '{}'", TypeName<T>());

            T& component = m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
            return component;
        }

        template<typename T, typename... Args>
        T& AddOrReplaceComponent(Args&&... args)
        {
            EX_ASSERT(m_scene, "Null pointer to scene in Entity: {}", GetName());
            T& component = m_scene->m_registry.emplace_or_replace<T>(m_entityHandle, std::forward<Args>(args)...);
            return component;
        }

        template<typename T>
        bool HasComponent()
        {
            EX_ASSERT(m_scene, "Null pointer to scene in Entity: {}", GetName());
            return m_scene->m_registry.all_of<T>(m_entityHandle);
        }

        template<typename T>
        T& GetComponent()
        {
            EX_ASSERT(!HasComponent<T>(), "Entity doesn't have this component: {}", TypeName<T>());
            return m_scene->m_registry.get<T>(m_entityHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            EX_ASSERT(!HasComponent<T>(), "Entity doesn't have this component: {}", TypeName<T>());
            return m_scene->m_registry.remove<T>(m_entityHandle);
        }

        operator bool() const { return m_entityHandle != entt::null; }
        operator entt::entity() const { return m_entityHandle; }
        operator Uint32() const { return (Uint32)m_entityHandle; }

        UUID GetUUID() { return GetComponent<IDComponent>().id; }
        const String& GetName() { return GetComponent<TagComponent>().tag; }
        
    private:       
        entt::entity m_entityHandle{entt::null};
        Scene* m_scene{nullptr};
    };
}