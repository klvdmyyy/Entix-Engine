// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/Memory.h"
#include "Core/String.h"
#include "Core/Types.h"

#include "Math/Vector.h"
#include "Math/MatrixTransform.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"

namespace ERUNTIME_NAMESPACE {
    struct IDComponent {
        UUID id;

        IDComponent() = default;
        IDComponent(const IDComponent&) = default;
        IDComponent(UUID uuid) : id(uuid) {}
    };

    struct TagComponent {
        String tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const String& tag)
            : tag(tag)
        {
        }
    };

    struct TransformComponent {
    public:
        Float3 position;
        Float3 rotation;
        Float3 scale;

        TransformComponent()
            : position(Float3(0.0f, 0.0f, 0.0f)),
              rotation(Float3(0.0f, 0.0f, 0.0f)),
              scale(Float3(1.0f, 1.0f, 1.0f))
        {
        }

        TransformComponent(const TransformComponent&) = default;

        TransformComponent(Float3 position,
                           Float3 rotation,
                           Float3 scale)
            : position(position), rotation(rotation), scale(scale)
        {
        }

        [[nodiscard]]
        Float4x4 GetLocalMatrix() const noexcept;

        [[nodiscard]]
        Float4x4 GetWorldMatrix(const Float4x4& parentWorld) const noexcept;
    };

    struct StaticMeshComponent {
        Ref<Renderer::VertexArray> vertexArray;

        struct Material {
            String shader{};
        } material{};

        StaticMeshComponent()
            : vertexArray(nullptr)
        {
        }

        StaticMeshComponent(const Ref<Renderer::VertexArray> vertexArray)
            : vertexArray(vertexArray)
        {
        }
    };

    struct CameraComponent {
    public:
        float fov = 45.0f;
        bool primary = true;
        bool fixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        void Update(const TransformComponent& transform, float aspect);

        [[nodiscard]]
        inline const Float4x4& GetView() const noexcept
        {
            return m_view;
        }

        [[nodiscard]]
        inline const Float4x4& GetProjection() const noexcept
        {
            return m_projection;
        }

    private:
        Float3 m_front = Float3(0.0f, 0.0f, -1.0f);
        Float3 m_right;

        static constexpr float YAW = -90.0f;
        static constexpr float PITCH = 0.0f;

        float m_yaw = YAW;
        float m_pitch = PITCH;

        Float3 m_up;
        Float3 m_worldUp = Float3(0.0f, 1.0f, 0.0f);

        Float4x4 m_view;
        Float4x4 m_projection;
    };
}