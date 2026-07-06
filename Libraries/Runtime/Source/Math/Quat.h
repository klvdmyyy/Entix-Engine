// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"

#include "Math/Matrix.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

using Quat = glm::quat;

namespace Math {
    FORCE_INLINE
    inline constexpr Float4x4 ToFloat4x4(const Quat& q)
    {
        return glm::toMat4(q);
    }
}