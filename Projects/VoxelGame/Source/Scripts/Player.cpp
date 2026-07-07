#include "Player.h"

void Player::OnCreate()
{
}

void Player::OnTick(Timestep deltaTime)
{
    auto& transform = GetComponent<TransformComponent>();

    transform.rotation.z += rotationSpeed * (float)deltaTime;
}

void Player::OnDestroy()
{
}