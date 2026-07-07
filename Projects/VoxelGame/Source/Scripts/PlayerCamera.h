// -*- mode: c++; -*-
#pragma once

#include <GameFramework.h>

class PlayerCamera : public ScriptableEntity {
protected:
    void OnTick(Timestep deltaTime) final;
};