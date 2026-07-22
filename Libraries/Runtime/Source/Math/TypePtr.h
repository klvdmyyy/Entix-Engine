// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Math {
    template<typename GenType>
    FORCE_INLINE
    inline typename GenType::value_type const* ValuePtr(GenType const& v)
    {
        return glm::value_ptr(v);
    }

    template<typename GenType>
    FORCE_INLINE
    inline typename GenType::value_type* ValuePtr(GenType& v)
    {
        return glm::value_ptr(v);
    }
}