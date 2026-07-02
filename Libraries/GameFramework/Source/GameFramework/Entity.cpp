#include "GameFramework/Entity.h"

Entity::Entity(entt::entity handle, Scene* scene)
    : m_entityHandle(handle), m_scene(scene)
{
}