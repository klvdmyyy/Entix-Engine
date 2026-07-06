#include "GameLayer.h"

GameLayer::GameLayer()
    : Layer("GameLayer")
{
}

void GameLayer::OnTick(float deltaTime)
{
    Application::Get().GetCurrentScene().OnTick(deltaTime);
}

void GameLayer::OnRender()
{
    Application::Get().GetCurrentScene().OnRender();
}