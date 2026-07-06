// -*- mode: c++; -*-
#pragma once

#include <GameFramework.h>

class PlayerController : public ScriptableEntity {
protected:
    void OnCreate() final
    {
    }

    void OnTick(float deltaTime) final
    {
        auto& transform = GetComponent<TransformComponent>();
    }

    void OnDestroy() final
    {
    }

private:
    float m_movementSpeed = 1.0f;
};