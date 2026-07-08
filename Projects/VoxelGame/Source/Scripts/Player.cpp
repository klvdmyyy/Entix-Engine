#include "Player.h"

#include <WSI/ActionSystem.h>
#include <WSI/Input.h>

#include <cmath>

Player::Player()
{
    EventBus::Instance().AddListener(this);
}

Player::~Player()
{
    EventBus::Instance().RemoveListener(this);
}

void Player::OnEvent(const Event& event)
{
    EventDispatcher dispatcher(event);

    dispatcher.Dispatch<MouseMotionEvent>([&](const MouseMotionEvent& event)
    {
        camera->yaw += event.k_xPosition * sensitivity;
        camera->pitch += event.k_yPosition * sensitivity;
    });
}

void Player::OnCreate()
{
    Application::Get().GetWindow()->GrabCursor(true);

    scene = &Application::Get().GetCurrentScene();
    
    transform = &GetComponent<TransformComponent>();

    cameraTransform = &scene->FindEntityByName("Camera")
        .GetComponent<TransformComponent>();

    camera = &scene->FindEntityByName("Camera")
        .GetComponent<CameraComponent>();
}

void Player::OnTick(Timestep deltaTime)
{
    // Movement
    
    Float3 dirF = camera->GetFront();
    Float3 dirR = camera->GetRight();
    
    Float3 movementF = dirF * movementSpeed;
    Float3 movementR = dirR * movementSpeed;

    if(ActionSystem::Instance().IsPressed("Menu"))
        Application::Get().Quit();

    if(ActionSystem::Instance().IsHeld("MoveForward"))
        transform->position -= movementF * (float)deltaTime;

    if(ActionSystem::Instance().IsHeld("MoveBackward"))
        transform->position += movementF * (float)deltaTime;

    if(ActionSystem::Instance().IsHeld("MoveLeft"))
        transform->position += movementR * (float)deltaTime;

    if(ActionSystem::Instance().IsHeld("MoveRight"))
        transform->position -= movementR * (float)deltaTime;
}

void Player::OnDestroy()
{
}