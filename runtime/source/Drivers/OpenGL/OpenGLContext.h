// -*- mode: c++; -*-
#pragma once

#include "Renderer/Context.h"
#include "Renderer/VertexArray.h"

#include <glad/glad.h>
#include <SDL3/SDL.h>

namespace ERUNTIME_NAMESPACE
{
    class ERUNTIME_API OpenGLContext : public Context
    {
    public:
        OpenGLContext(const Ref<Window>& window);
        ~OpenGLContext();

        [[nodiscard]]
        VertexArray* CreateVertexArray() final;
        
        [[nodiscard]]
        Shader* CreateShader(const String& filepath) final;

        [[nodiscard]]
        VertexBuffer* CreateVertexBuffer(float* data, uint32_t size) final;

        [[nodiscard]]
        IndexBuffer* CreateIndexBuffer(uint32_t* indices, uint32_t count) final;

        void BeginScene() final {}
        void EndScene() final {}

        void SetClearColor(float r, float g, float b) final;
        void Clear() final;

        void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array) final;

        void Swap() final;

        void InitGUI() final;

    private:
        Ref<Window> m_window;
        SDL_GLContext m_context;
    };
}