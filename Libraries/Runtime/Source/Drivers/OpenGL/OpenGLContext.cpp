#include "Drivers/OpenGL/OpenGLContext.h"

#include "Core/Assert.h"

#include "Drivers/OpenGL/OpenGLBuffer.h"
#include "Drivers/OpenGL/OpenGLShader.h"
#include "Drivers/OpenGL/OpenGLVertexArray.h"
#include "Drivers/OpenGL/OpenGLTexture.h"
#include "Drivers/OpenGL/OpenGLFramebuffer.h"

#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

static constexpr const char* GUI_GLSL_VERSION = "#version 130";

OpenGLContext::OpenGLContext(const Ref<Window>& window)
    : m_window(window)
{
    ZoneScoped;
    
    m_context = SDL_GL_CreateContext(static_cast<SDL_Window*>(m_window->GetWindowHandle()));

    EX_ASSERT(m_context, "failed to create opengl context. sdl error: {}", SDL_GetError());

    SDL_GL_MakeCurrent(static_cast<SDL_Window*>(m_window->GetWindowHandle()), m_context);

    EX_ASSERT(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress), "failed to load opengl loader!");

    TracyGpuContext;

    glEnable(GL_DEPTH_TEST);
}

OpenGLContext::~OpenGLContext()
{
    SDL_GL_DestroyContext(m_context);
}

void OpenGLContext::BeginScene(const Viewport& viewport)
{
    TracyGpuZone("Begin Scene");
    glViewport(
        static_cast<Int32>(viewport.GetAbsolutePosition().x),
        static_cast<Int32>(viewport.GetAbsolutePosition().y),
        static_cast<Int32>(viewport.GetAbsoluteSize().x),
        static_cast<Int32>(viewport.GetAbsoluteSize().y)
    );
    glScissor(
        static_cast<Int32>(viewport.GetAbsolutePosition().x),
        static_cast<Int32>(viewport.GetAbsolutePosition().y),
        static_cast<Int32>(viewport.GetAbsoluteSize().x),
        static_cast<Int32>(viewport.GetAbsoluteSize().y)
    );
    glEnable(GL_SCISSOR_TEST);
}

void OpenGLContext::EndScene()
{
    TracyGpuZone("End Scene");
    glDisable(GL_SCISSOR_TEST);
}

void OpenGLContext::SetClearColor(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0f);
}

void OpenGLContext::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLContext::Swap()
{
    TracyGpuCollect;
    SDL_GL_SwapWindow(static_cast<SDL_Window*>(m_window->GetWindowHandle()));
}

void OpenGLContext::Submit(Shader* shader, VertexArray* vertexArray)
{
    TracyGpuZone("Sumbit mesh");

    shader->Bind();
    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES,
                   vertexArray->GetIndexBuffer()->GetCount(),
                   GL_UNSIGNED_INT, 0);
}

void OpenGLContext::InitGUI(ImGuiContext* ctx)
{
    ImGui::SetCurrentContext(ctx);

    ImGui_ImplSDL3_InitForOpenGL(static_cast<SDL_Window*>(m_window->GetWindowHandle()), m_context);
    ImGui_ImplOpenGL3_Init(GUI_GLSL_VERSION);

    m_window->EnableGUIUpdate();
}

void OpenGLContext::ShutdownGUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
}

void OpenGLContext::NewGUIFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
}

void OpenGLContext::RenderGUI()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void OpenGLContext::Restore()
{
    SDL_GL_MakeCurrent(static_cast<SDL_Window*>(m_window->GetWindowHandle()), m_context);
}

[[nodiscard]]
VertexArray* OpenGLContext::CreateVertexArray(const ResourceId& id)
{
    return new OpenGLVertexArray(id);
}
    
[[nodiscard]]
Shader* OpenGLContext::CreateShader(const ResourceId& id)
{
    return new OpenGLShader(id);
}

[[nodiscard]]
VertexBuffer* OpenGLContext::CreateVertexBuffer(float* data, uint32_t size)
{
    return new OpenGLVertexBuffer(data, size);
}

[[nodiscard]]
IndexBuffer* OpenGLContext::CreateIndexBuffer(uint32_t* indices, uint32_t count)
{
    return new OpenGLIndexBuffer(indices, count);
}

[[nodiscard]]
Texture* OpenGLContext::CreateTexture(const ResourceId& id, const TextureSpecification& spec)
{
    return new OpenGLTexture(id, spec);
}

[[nodiscard]]
Framebuffer* OpenGLContext::CreateFramebuffer(const FramebufferSpecification& spec)
{
    return new OpenGLFramebuffer(spec);
}
