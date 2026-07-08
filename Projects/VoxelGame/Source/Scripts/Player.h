// -*- mode: c++; -*-
#pragma once

#include <GameFramework.h>

class Player : public ScriptableEntity {
public:
    float movementSpeed = 1.0f;

protected:
    void OnCreate() final;
    void OnTick(Timestep deltaTime) final;
    void OnDestroy() final;

private:
    Scene* scene = nullptr;
    
    TransformComponent* transform = nullptr;
    CameraComponent* playerCamera = nullptr;
};