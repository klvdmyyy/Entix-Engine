#include "Core/Application.h"

#include <SDL3/SDL.h>

using namespace Core;

void Application::Init()
{
	SDL_Init(SDL_INIT_VIDEO);
}

void Application::Shutdown()
{
	SDL_Quit();
}