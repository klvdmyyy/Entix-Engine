#pragma once

#include "Entix/Core/Result.h"

namespace Entix
{
    struct ApplicationConfig
    {
    };

    class Application
    {
    public:
        Application();
        ~Application();

        Result<void> Run();

    private:
        Result<void> Initialize();
        Result<void> Shutdown();
    };
}