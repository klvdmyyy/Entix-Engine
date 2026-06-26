// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/Memory.h"

#include "Math/Vector.h"

#include "Renderer/VertexArray.h"

namespace ERUNTIME_NAMESPACE {
    struct Transform {
        Float3 position;
        Float3 rotation;
        Float3 scale;
    };

    struct Mesh {
        Ref<VertexArray> vertexArray;

        struct {
            // TODO...
        } material;
    };
}