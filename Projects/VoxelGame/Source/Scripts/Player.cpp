#include "Player.h"

#include <WSI/ActionSystem.h>

void Player::OnCreate()
{
}

void Player::OnTick(Timestep deltaTime)
{
    auto& transform = GetComponent<TransformComponent>();

    if(ActionSystem::Instance().IsHeld("MoveForward"))
        transform.position.z -= 2.0f * (float)deltaTime;

    if(ActionSystem::Instance().IsHeld("MoveBackward"))
        transform.position.z += 2.0f * (float)deltaTime;
}

void Player::OnDestroy()
{
}