// -*- mode: c++; -*-
#pragma once

#include "Core/String.h"

class Layer {
public:
    Layer(const String& name)
        : k_debugName(name)
    {
    }
    virtual ~Layer() = default;

    virtual void OnAttach() { }
    virtual void OnDetach() { }
    virtual void OnTick([[maybe_unused]] float deltaTime) { }
    
    virtual void OnPreRender() { }
    virtual void OnRender() { }
    virtual void OnPostRender() { }

    const String& GetName() const noexcept { return k_debugName; }

protected:
    const String k_debugName;
};