#include "Scene/Entity.h"

namespace ERUNTIME_NAMESPACE {
    Entity::Entity(entt::entity handle, Scene* scene)
        : m_entityHandle(handle), m_scene(scene)
    {
    }
}