#include "Player.h"

#include <WSI/ActionSystem.h>

void Player::OnCreate()
{
    scene = &Application::Get().GetCurrentScene();
    
    transform = &GetComponent<TransformComponent>();
    
    playerCamera = &scene->FindEntityByName("Camera")
        .GetComponent<CameraComponent>();
}

void Player::OnTick(Timestep deltaTime)
{
    Float3 dir = playerCamera->GetFront();
    Float3 movement = dir * movementSpeed;

    if(ActionSystem::Instance().IsHeld("MoveForward"))
        transform->position -= movement * (float)deltaTime;

    if(ActionSystem::Instance().IsHeld("MoveBackward"))
        transform->position += movement * (float)deltaTime;
}

void Player::OnDestroy()
{
}