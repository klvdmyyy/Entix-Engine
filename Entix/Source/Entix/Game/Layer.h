#pragma once

#include "Entix/Game/ControlFlow.h"
#include "Entix/Game/WorldContext.h"

namespace Entix
{
    class Layer
    {
    public:
        virtual ~Layer() = default;

        virtual void OnAttach() { }
        virtual void OnDetach() { }

        virtual void OnTick() { }
        virtual void OnPreRender() { }
        virtual void OnRender() { }
        virtual void OnPostRender() { }

    protected:
        inline ControlFlow& GetControlFlow() const noexcept { return *m_controlFlow; }
        inline WorldContext& GetWorldContext() const noexcept { return *m_worldContext; }

    private:
        friend class LayerStack;

        ControlFlow* m_controlFlow = nullptr;
        WorldContext* m_worldContext = nullptr;
    };
}
