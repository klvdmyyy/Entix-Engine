// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"

#include "Math/Matrix.h"
#include "Math/Vector.h"

#include <glm/gtc/matrix_transform.hpp>

namespace ERUNTIME_NAMESPACE::Math {
    FORCE_INLINE
    inline constexpr Float4x4 Translate(const Float4x4& m, const Float3& v)
    {
        return glm::translate(m, v);
    }

    FORCE_INLINE
    inline constexpr Float4x4 Rotate(const Float4x4& m, float angle, const Float3& axis)
    {
        return glm::rotate(m, angle, axis);
    }

    FORCE_INLINE
    inline constexpr Float4x4 Scale(const Float4x4& m, const Float3& v)
    {
        return glm::scale(m, v);
    }

    FORCE_INLINE
    inline constexpr Float4x4 LookAt(const Float3& eye, const Float3& center,
                                     const Float3& up)
    {
        return glm::lookAt(eye, center, up);
    }

    FORCE_INLINE
    inline constexpr Float3 Normalize(const Float3& v)
    {
        return glm::normalize(v);
    }
    
    FORCE_INLINE
    inline constexpr Float3 Cross(const Float3& x, const Float3& y)
    {
        return glm::cross(x, y);
    }

    FORCE_INLINE
    inline constexpr float Radians(float degrees)
    {
        return glm::radians(degrees);
    }

    FORCE_INLINE
    inline constexpr float Cos(float v)
    {
        return cos(v);
    }

    FORCE_INLINE
    inline constexpr float Sin(float v)
    {
        return sin(v);
    }

    FORCE_INLINE
    inline constexpr Float4x4 Perspective(float fovy, float aspect, float zNear, float zFar)
    {
        return glm::perspective(fovy, aspect, zNear, zFar);
    }
}