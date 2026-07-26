#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Result.h"

namespace Entix
{
    struct ApplicationConfig
    {
    };

    class Application
    {
    public:
        ENTIX_API Application();
        ENTIX_API ~Application();

        ENTIX_API Result<void> Run();

    private:
        ENTIX_API Result<void> Initialize();
        ENTIX_API Result<void> Shutdown();

        bool m_quit;
    };
}