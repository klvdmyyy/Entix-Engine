// -*- mode: c++; -*-
#pragma once

#include "Renderer/Context.h"
#include "Renderer/VertexArray.h"

#include <glad/glad.h>
#include <SDL3/SDL.h>

#ifndef ERUNTIME_BUILD_DLL
#warning "You shouldn't include Driver headers from public API!"
#endif

// We don't include that in public API. We just can use `using namespace`
using namespace Renderer;

class ERUNTIME_API OpenGLContext : public Context
{
 public:
    OpenGLContext(const Ref<Window>& window);
    ~OpenGLContext();

    [[nodiscard]]
    VertexArray* CreateVertexArray(const ResourceId& id) final;
        
    [[nodiscard]]
    Shader* CreateShader(const ResourceId& id) final;

    [[nodiscard]]
    VertexBuffer* CreateVertexBuffer(float* data, uint32_t size) final;

    [[nodiscard]]
    IndexBuffer* CreateIndexBuffer(uint32_t* indices, uint32_t count) final;

    [[nodiscard]]
    Texture* CreateTexture(const ResourceId& id, const TextureSpecification& spec) final;
   
    [[nodiscard]]
    Framebuffer* CreateFramebuffer(const FramebufferSpecification& spec) final;

    void BeginScene(const Viewport& viewport) final;
    void EndScene() final;

    void SetClearColor(float r, float g, float b) final;
    void Clear() final;

    void Submit(Shader* shader, VertexArray* vertex_array) final;

    void Swap() final;

    void InitGUI(ImGuiContext* ctx) final;
    void ShutdownGUI() final;
    void NewGUIFrame() final;
    void RenderGUI() final;

    void Restore() final;

 private:
    Ref<Window> m_window;
    SDL_GLContext m_context;
};