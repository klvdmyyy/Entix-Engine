#include "Player.h"

#include <WSI/ActionSystem.h>

void Player::OnCreate()
{
}

void Player::OnTick(Timestep deltaTime)
{
    auto& transform = GetComponent<TransformComponent>();
    Entity playerCamera = Application::Get().GetCurrentScene().FindEntityByName("Camera");

    Float3 dir = playerCamera.GetComponent<CameraComponent>().GetFront();

    if(ActionSystem::Instance().IsHeld("MoveForward"))
        transform.position += dir * (float)deltaTime;

    if(ActionSystem::Instance().IsHeld("MoveBackward"))
        transform.position -= dir * (float)deltaTime;
}

void Player::OnDestroy()
{
}