// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/String.h"

namespace ERUNTIME_NAMESPACE::GUI {
    class Widget {
    public:
        Widget(const String& name);

        virtual void Draw() = 0;

        String GetName() const noexcept { return k_name; }

    private:
        const String k_name;
    };
}