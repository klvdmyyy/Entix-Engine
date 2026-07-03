#include "Drivers/OpenGL/OpenGLContext.h"

#include "Core/Assert.h"

#include "Drivers/OpenGL/OpenGLBuffer.h"
#include "Drivers/OpenGL/OpenGLShader.h"
#include "Drivers/OpenGL/OpenGLVertexArray.h"

#include <tracy/TracyOpenGL.hpp>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

static constexpr const char* GUI_GLSL_VERSION = "#version 130";

OpenGLContext::OpenGLContext(const Ref<Window>& window)
    : m_window(window)
{
    m_context = SDL_GL_CreateContext(static_cast<SDL_Window*>(m_window->GetWindowHandle()));

    EX_ASSERT(m_context, "failed to create opengl context. sdl error: {}", SDL_GetError());

    SDL_GL_MakeCurrent(static_cast<SDL_Window*>(m_window->GetWindowHandle()), m_context);

    EX_ASSERT(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress), "failed to load opengl loader!");
}

OpenGLContext::~OpenGLContext()
{
    SDL_GL_DestroyContext(m_context);
}

void OpenGLContext::SetClearColor(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0f);
}

void OpenGLContext::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLContext::Swap()
{
    SDL_GL_SwapWindow(static_cast<SDL_Window*>(m_window->GetWindowHandle()));
}

void OpenGLContext::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray)
{
    shader->Bind();
    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES,
                   vertexArray->GetIndexBuffer()->GetCount(),
                   GL_UNSIGNED_INT, 0);
}

void OpenGLContext::InitGUI()
{
    ImGui_ImplSDL3_InitForOpenGL(static_cast<SDL_Window*>(m_window->GetWindowHandle()), m_context);
    ImGui_ImplOpenGL3_Init(GUI_GLSL_VERSION);
}


[[nodiscard]]
VertexArray* OpenGLContext::CreateVertexArray()
{
    return new OpenGLVertexArray();
}
    
[[nodiscard]]
Shader* OpenGLContext::CreateShader(IO::Reader& shaderReader)
{
    return new OpenGLShader(shaderReader);
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