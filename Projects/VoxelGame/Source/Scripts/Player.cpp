#include "Player.h"

#include <Input/Events.h>
#include <Input/Actions.h>

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

    if(Input::IsActionPressed("Menu"))
        Application::Get().Quit();

    if(Input::IsActionHeld("MoveForward"))
        transform->position -= movementF * (float)deltaTime;

    if(Input::IsActionHeld("MoveBackward"))
        transform->position += movementF * (float)deltaTime;

    if(Input::IsActionHeld("MoveLeft"))
        transform->position += movementR * (float)deltaTime;

    if(Input::IsActionHeld("MoveRight"))
        transform->position -= movementR * (float)deltaTime;
}

void Player::OnDestroy()
{
}