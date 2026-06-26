// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/Types.h"

namespace ERUNTIME_NAMESPACE {
    class Scene {
    public:
        Scene();
        ~Scene();

        void OnTick(float deltaTime);
    };
}