// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/String.h"

#include "Core/Types.h"

namespace ERUNTIME_NAMESPACE::GUI {
    class Window {
    public:
        Window(const String& name, Uint32 startWidth = 200, Uint32 startHeight = 200, Uint32 startXPos = 10, Uint32 startYPos = 10);

        virtual void Draw() = 0;

        void Show();
        void Hide();

        bool IsOpened() const noexcept { return m_show; }
        String GetName() const noexcept { return k_name; }

    private:
        friend class Context;

        Uint32 GetStartWidth() const { return k_startWidth; }
        Uint32 GetStartHeight() const { return k_startHeight; }
        Uint32 GetStartXPos() const { return k_startXPos; }
        Uint32 GetStartYPos() const { return k_startYPos; }
        
        void BeginRender();
        void EndRender();

        const String k_name;
        bool m_show;
        bool m_isOpen;

        const Uint32 k_startWidth;
        const Uint32 k_startHeight;
        const Uint32 k_startXPos;
        const Uint32 k_startYPos;
    };
}