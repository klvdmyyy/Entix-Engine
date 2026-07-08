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
    });
}

void Player::OnCreate()
{
    scene = &Application::Get().GetCurrentScene();
    
    transform = &GetComponent<TransformComponent>();

    cameraTransform = &scene->FindEntityByName("Camera")
        .GetComponent<TransformComponent>();

    camera = &scene->FindEntityByName("Camera")
        .GetComponent<CameraComponent>();
}

void Player::OnTick(Timestep deltaTime)
{
    // Mouse motion

    float xPos = 0.0f;
    float yPos = 0.0f;

    Input::GetCursorPosition(xPos, yPos);

    if(m_firstMouseMotion) {
        m_lastX = xPos;
        m_lastY = yPos;
        m_firstMouseMotion = false;
    }

    m_xOffset = xPos - m_lastX;
    m_yOffset = m_lastY - yPos;

    m_lastX = xPos;
    m_lastY = yPos;

    m_xOffset *= sensitivity;
    m_yOffset *= sensitivity;

    camera->m_yaw += m_xOffset;
    camera->m_pitch += m_yOffset;

    // Movement
    
    Float3 dirF = camera->GetFront();
    Float3 dirR = camera->GetRight();
    
    Float3 movementF = dirF * movementSpeed;
    Float3 movementR = dirR * movementSpeed;

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