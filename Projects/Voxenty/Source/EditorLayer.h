#pragma once

#include <GameFramework.h>

class EditorLayer : public Layer {
public:
    EditorLayer();

    void OnRender() final;
};