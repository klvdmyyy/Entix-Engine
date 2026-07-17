#pragma once

#include <GameFramework.h>

inline StaticMeshComponent CreateSquareMesh(Renderer::Context* rendererContext)
{    
    auto vertex_array = Ref<Renderer::VertexArray>(rendererContext->CreateVertexArray());

    float vertices[] = {
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };

    auto vertex_buffer = Ref<Renderer::VertexBuffer>(rendererContext->CreateVertexBuffer(vertices, sizeof(vertices)));

    Renderer::BufferLayout layout = {
        { "a_Position", Renderer::ShaderDataType::Float3 },
    };

    vertex_buffer->SetLayout(layout);

    uint32_t indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    auto index_buffer = Ref<Renderer::IndexBuffer>(rendererContext->CreateIndexBuffer(indices, sizeof(indices)));

    vertex_array->AddVertexBuffer(vertex_buffer);
    vertex_array->SetIndexBuffer(index_buffer);

    /* ActionSystem::Instance().PushContext(ActionContext{"Console"}); */
    /* ActionSystem::Instance().SetActionMap(ActionMap::LoadFromFile(VBOX_DEFAULT_ACTION_MAP)); */

    StaticMeshComponent mesh;
    mesh.vertexArray = vertex_array;

    return mesh;
}