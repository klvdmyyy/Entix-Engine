// -*- mode: c++; -*-
#pragma once

#include <GameFramework.h>

class GameLayer : public Layer {
public:
    GameLayer();

    void OnTick(float deltaTime) final;
    void OnRender() final;
};