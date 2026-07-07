// -*- mode: c++; -*-
#pragma once

#include "GameFramework/Entity.h"

#include "Core/Timestep.h"

class ScriptableEntity {
public:
    virtual ~ScriptableEntity() = default;

    template<typename T>
    T& GetComponent()
    {
        return m_entity.GetComponent<T>();
    }

protected:
    virtual void OnCreate() { }
    virtual void OnDestroy() { }
    virtual void OnTick(Timestep deltaTime) { }

private:
    friend class Scene;
    
    Entity m_entity;
};