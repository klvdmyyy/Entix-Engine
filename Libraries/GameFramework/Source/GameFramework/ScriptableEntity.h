// -*- mode: c++; -*-
#pragma once

#include "GameFramework/Entity.h"

class ScriptableEntity {
public:
    virtual ~ScriptableEntity();

    template<typename T>
    T& GetComponent()
    {
        return m_entity.GetComponent<T>();
    }

protected:
    virtual void OnCreate() { }
    virtual void OnDestroy() { }
    virtual void OnTick(float deltaTime) { }

private:
    friend class Scene;
    
    Entity m_entity;
};