// -*- mode: c++; -*-
#pragma once

#include <GameFramework.h>

class Player : public ScriptableEntity, EventListener {
public:
    float sensitivity = 0.05f;
    float movementSpeed = 1.0f;

    Player();
    ~Player();

protected:
    void OnCreate() final;
    void OnTick(Timestep deltaTime) final;
    void OnDestroy() final;

    void OnEvent(const Event& event) final;

private:
    bool m_firstMouseMotion = true;
    
    float m_lastX = 0.0f;
    float m_lastY = 0.0f;

    float m_xOffset = 0.0f;
    float m_yOffset = 0.0f;
    
    Scene* scene = nullptr;
    
    TransformComponent* transform = nullptr;
    
    TransformComponent* cameraTransform = nullptr;
    CameraComponent* camera = nullptr;
};