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
        return {};
    }

    Result<void> Application::Run()
    {
        EX_TRY(Initialize());

        EX_TRY(Shutdown());

        return {};
    }

    Result<void> Application::Shutdown()
    {
        return {};
    }
}