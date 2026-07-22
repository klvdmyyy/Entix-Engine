// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/Memory.h"
#include "Core/String.h"
#include "Core/Types.h"

#include "Math/Vector.h"
#include "Math/MatrixTransform.h"

#include "Resources/ResourceHandle.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Viewport.h"
#include "Renderer/Framebuffer.h"

#include <entt/entt.hpp>

#include <optional>

class Entity;

struct IdComponent {
    Uuid id;

    IdComponent() = default;
    IdComponent(const IdComponent&) = default;
    IdComponent(Uuid uuid) : id(uuid) {}
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
    Float3 position = Float3(0.0f, 0.0f, 0.0f);
    Float3 rotation = Float3(0.0f, 0.0f, 0.0f);
    Float3 scale = Float3(1.0f, 1.0f, 1.0f);

    entt::entity parent = entt::null;

    TransformComponent() = default;

    TransformComponent(const TransformComponent&) = default;

    TransformComponent(Float3 position,
                       Float3 rotation,
                       Float3 scale)
        : position(position), rotation(rotation), scale(scale)
    {
    }

    TransformComponent(Entity parentEntity);

    void UpdateLocalMatrix();
    void UpdateWorldMatrix(const Float4x4& parentWorld);

    void SetLocalMatrix(const Float4x4 &) noexcept;

    [[nodiscard]]
    const Float4x4& GetLocalMatrix() const noexcept;

    [[nodiscard]]
    const Float4x4& GetWorldMatrix() const noexcept;

    [[nodiscard]]
    Float3 GetLocalPosition() const noexcept;

    [[nodiscard]]
    Float3 GetLocalRotation() const noexcept;

    [[nodiscard]]
    Float3 GetLocalScale() const noexcept;

    [[nodiscard]]
    Float3 GetWorldPosition() const noexcept;

    [[nodiscard]]
    Float3 GetWorldRotation() const noexcept;

    [[nodiscard]]
    Float3 GetWorldScale() const noexcept;

private:
    bool m_localMatrixDirty = false;
    
    Float4x4 m_localMatrix = Float4x4(1.0f);
    Float4x4 m_worldMatrix = Float4x4(1.0f);
};

struct StaticMeshComponent {
    StaticMeshComponent()
        : vertexArray()
    {
    }

    ResourceHandle<Renderer::VertexArray> vertexArray;

    struct Material {
        ResourceHandle<Renderer::Texture> texture{};
        ResourceHandle<Renderer::Shader> shader{};
    } material{};
};

struct CameraComponent {
public:
    Renderer::Viewport viewport{};
    Ref<Renderer::Framebuffer> framebuffer = nullptr;

    float fov = 45.0f;
    
    float yaw = DEFAULT_YAW;
    float pitch = DEFAULT_PITCH;

    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;

    void SetViewport(const Renderer::Viewport& vp) noexcept { viewport = vp; }
    void Update(const TransformComponent& transform);

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

    [[nodiscard]]
    inline const Float3& GetFront() const noexcept {
        return m_front;
    }

    [[nodiscard]]
    inline const Float3& GetRight() const noexcept {
        return m_right;
    }

private:
    Float3 m_front = Float3(0.0f, 0.0f, -1.0f);
    Float3 m_right;

    static constexpr float DEFAULT_YAW = -90.0f;
    static constexpr float DEFAULT_PITCH = 0.0f;

    Float3 m_up;
    Float3 m_worldUp = Float3(0.0f, 1.0f, 0.0f);

    Float4x4 m_view;
    Float4x4 m_projection;
};

class ScriptableEntity;

struct NativeScriptComponent {
    using InstantiateFunction = std::function<Scope<ScriptableEntity>()>;
    using DestroyFunction = std::function<void(NativeScriptComponent*)>;
    
    Scope<ScriptableEntity> instance;

    InstantiateFunction InstantiateScript;
    DestroyFunction DestroyScript;

    template<typename T>
    void Bind()
    {
        InstantiateScript = [](){ return Scope<ScriptableEntity>(new T()); };
        DestroyScript = [](NativeScriptComponent* nsc) { nsc->instance.reset(nullptr); };
    }

    NativeScriptComponent() = default;
};
