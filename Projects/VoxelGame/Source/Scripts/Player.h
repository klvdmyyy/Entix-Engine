// -*- mode: c++; -*-
#pragma once

#include <GameFramework.h>

class Player : public ScriptableEntity {
public:

protected:
    void OnCreate() final;
    void OnTick(Timestep deltaTime) final;
    void OnDestroy() final;

private:
};