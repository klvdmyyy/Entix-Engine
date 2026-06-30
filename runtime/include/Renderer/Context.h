// -*- mode: c++; -*-
#pragma once

#include "Core/Memory.h"

#include "WSI/Window.h"

#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

namespace ERUNTIME_NAMESPACE::Renderer
{
    class ERUNTIME_API Context
    {
    public:
        virtual ~Context() = default;

        [[nodiscard]]
        static Context* Create(const Ref<Window>& window);

        [[nodiscard]]
        virtual VertexArray* CreateVertexArray() = 0;

        [[nodiscard]]
        virtual Shader* CreateShader(const String& filepath) = 0;

        [[nodiscard]]
        virtual VertexBuffer* CreateVertexBuffer(float* data, uint32_t size) = 0;

        [[nodiscard]]
        virtual IndexBuffer* CreateIndexBuffer(uint32_t* indices, uint32_t count) = 0;

        virtual void BeginScene() = 0;
        virtual void EndScene() = 0;

        virtual void SetClearColor(float r = 1.0f, float g = 1.0f, float b = 1.0f) = 0;
        virtual void Clear() = 0;

        virtual void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array) = 0;

        virtual void Swap() = 0;

        // ---------------------------------------------------------------------
        // Инициализация интерфейса при помощи контекста рендеринга
        // ---------------------------------------------------------------------
        virtual void InitGUI() = 0;

    protected:
        Context() = default;
    };
}