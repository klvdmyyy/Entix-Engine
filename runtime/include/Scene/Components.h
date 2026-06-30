// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/Memory.h"
#include "Core/String.h"
#include "Core/Types.h"

#include "Math/Vector.h"

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
        // SceneCamera camera;

        bool primary = true;
        bool fixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };
}