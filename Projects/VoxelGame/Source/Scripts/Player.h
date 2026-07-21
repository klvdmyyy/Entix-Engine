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
    bool m_grabCursor = false;

    Scene* scene = nullptr;
    
    TransformComponent* transform = nullptr;
    
    TransformComponent* cameraTransform = nullptr;
    CameraComponent* camera = nullptr;
};