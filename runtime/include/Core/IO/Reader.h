// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/String.h"

namespace ERUNTIME_NAMESPACE::IO {
    class Reader {
    public:
        virtual ~Reader() = default;

        virtual String ReadStr() = 0;
    };
}