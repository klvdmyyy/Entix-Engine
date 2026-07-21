#pragma once

#include "GameFramework/Components.h"

#include "Renderer/Context.h"

namespace Systems
{
    void StaticMeshRender(
        Renderer::Context* rendererContext,
        TransformComponent& transform,
        StaticMeshComponent& mesh,
        CameraComponent& camera
    );
}