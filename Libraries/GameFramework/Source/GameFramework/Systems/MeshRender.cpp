#include "GameFramework/Systems/MeshRender.h"

#include "Core/Assert.h"

#include <tracy/Tracy.hpp>

static constexpr const char* STATIC_MESH_FRAME = "StaticMesh rendering";

namespace Systems
{
    void StaticMeshRender(
        Renderer::Context* rendererContext,
        TransformComponent& transform,
        StaticMeshComponent& mesh,
        CameraComponent& camera
    )
    {
        FrameMarkStart(STATIC_MESH_FRAME);

        auto& shader = mesh.material.shader;
        auto& texture = mesh.material.texture;
        EX_DEBUG_ASSERT(shader.IsValid(), "Usage of invalid shader in material!");
        
        shader->Bind();
        
        shader->SetFloat4x4("model", transform.GetWorldMatrix());
        shader->SetFloat4x4("view", camera.GetView());
        shader->SetFloat4x4("projection", camera.GetProjection());

        if(texture) {
            texture->Bind();
            shader->SetInt("texture", 0);
        }
        
        rendererContext->Submit(shader.Get(), mesh.vertexArray.Get());
        FrameMarkEnd(STATIC_MESH_FRAME);
    }
}