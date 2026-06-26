// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/String.h"

namespace ERUNTIME_NAMESPACE::GUI {
    class Window {
    public:
        Window(const String& name);

        virtual void Draw() = 0;

        void Show();
        void Hide();

        String GetName() const noexcept { return k_name; }

    private:
        friend class Context;
        
        void BeginRender();
        void EndRender();

        const String k_name;
        bool m_show;
        bool m_isOpen;
    };
}