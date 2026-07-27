#pragma once

#include "Entix/Core/Types.h"

namespace Entix::RHI
{
    class Rect
    {
    public:
        Rect() = default;
        Rect(Uint32 x_, Uint32 y_) : x(x_), y(y_) {}

        Uint32 x = 0;
        Uint32 y = 0;
    };
}