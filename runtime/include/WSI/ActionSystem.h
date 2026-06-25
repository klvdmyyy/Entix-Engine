#pragma once

#include "Core/Base.h"
#include "Core/String.h"

#include "Core/Memory.h"

#include "Core/EventSystem.h"

#include "WSI/Input.h"

#include <mutex>
#include <unordered_set>
#include <vector>

namespace ERUNTIME_NAMESPACE {
    class ActionContext {
    public:
        void Activate();
        void Deactivate();

        bool IsActive() const;

        void AddAction(const String& name);
        bool HasAction(const String& name) const;
    };

    class ActionSystem {
    public:
        static ActionSystem& Instance();

        void PushContext(Scope<ActionContext> context);
        void PopContext();

        bool IsPressed(const String& name) const;
    };
}