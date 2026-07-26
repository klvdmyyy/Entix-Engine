#include "Entix/Game/Application.h"

namespace Entix
{
    Application::Application()
    {
    }

    Application::~Application()
    {
    }

    Result<void> Application::Initialize()
    {
    }

    Result<void> Application::Run()
    {
        EX_TRY(Initialize());

        EX_TRY(Shutdown());
    }

    Result<void> Application::Shutdown()
    {
    }
}