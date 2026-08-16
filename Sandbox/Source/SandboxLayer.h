#pragma once

#include "Entix/Game/Layer.h"

namespace Entix
{
    class SandboxLayer final : public Layer
    {
    public:
        void OnAttach() override
        {
            GetWorldContext().GetThreadPool().EnqueueToMainThread([](){
            });
            GetControlFlow().Quit();
        }
    };
}
