// -*- mode: c++; -*-
#pragma once

#include <GameFramework.h>

class GameLayer : public Layer {
public:
    GameLayer();

    void OnAttach() final;

    void OnTick(Timestep deltaTime) final;
    void OnRender() final;
};