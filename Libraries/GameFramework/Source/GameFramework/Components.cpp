#include "GameFramework/Components.h"

#include "GameFramework/Entity.h"

#include "Math/MatrixTransform.h"
#include "Math/Quat.h"

#include <limits>

#include <tracy/Tracy.hpp>

TransformComponent::TransformComponent(Entity parentEntity)
    : parent((entt::entity)parentEntity)
{
}

void TransformComponent::UpdateLocalMatrix()
{
    ZoneScoped;

    if(m_localMatrixDirty)
    {
        position = GetLocalPosition();
        rotation = GetLocalRotation();
        scale = GetLocalScale();
        
        m_localMatrixDirty = false;
    }

    Float4x4 rot = Math::ToFloat4x4(Quat(rotation));
    
    m_localMatrix = Math::Translate(Float4x4(1.0f), position)
        * rot * Math::Scale(Float4x4(1.0f), scale);
}

void TransformComponent::UpdateWorldMatrix(const Float4x4& parentWorld)
{
    ZoneScoped;

    m_worldMatrix = parentWorld * GetLocalMatrix();
}

void TransformComponent::SetLocalMatrix(const Float4x4 &mat) noexcept
{
    m_localMatrixDirty = true;
    m_localMatrix = mat;
}

const Float4x4& TransformComponent::GetLocalMatrix() const noexcept
{
    return m_localMatrix;
}

const Float4x4& TransformComponent::GetWorldMatrix() const noexcept
{
    return m_worldMatrix;
}

Float3 TransformComponent::GetLocalPosition() const noexcept {
    return Float3(m_localMatrix[3]);
}

Float3 TransformComponent::GetLocalRotation() const noexcept {
    Float3 localPos;
    Float3 localRot;
    Float3 localScale;
    Math::Decompose(m_localMatrix, localPos, localRot, localScale);
    return localRot;
}

Float3 TransformComponent::GetLocalScale() const noexcept {
    Float3 localScale;
    // Можно извлечь как длину колонок
    localScale.x = Math::Length(Float3(m_localMatrix[0]));
    localScale.y = Math::Length(Float3(m_localMatrix[1]));
    localScale.z = Math::Length(Float3(m_localMatrix[2]));
    return localScale;
}

Float3 TransformComponent::GetWorldPosition() const noexcept {
    return Float3(m_worldMatrix[3]);
}

Float3 TransformComponent::GetWorldRotation() const noexcept {
    Float3 worldPos;
    Float3 worldRot;
    Float3 worldScale;
    Math::Decompose(m_worldMatrix, worldPos, worldRot, worldScale);
    return worldRot;
}

Float3 TransformComponent::GetWorldScale() const noexcept {
    Float3 worldScale;
    // Можно извлечь как длину колонок
    worldScale.x = Math::Length(Float3(m_worldMatrix[0]));
    worldScale.y = Math::Length(Float3(m_worldMatrix[1]));
    worldScale.z = Math::Length(Float3(m_worldMatrix[2]));
    return worldScale;
}

void CameraComponent::Update(const TransformComponent& transform)
{
    ZoneScopedN("CameraComponent Update Method");
    
    const float k_aspect = viewport.GetAspectRatio();

    const float k_yaw = yaw + transform.GetWorldRotation().x;
    const float k_pitch = pitch + transform.GetWorldRotation().y;

    Float3 front;
    front.x = Math::Cos(Math::Radians(k_yaw)) * Math::Cos(Math::Radians(k_pitch));
    front.y = Math::Sin(Math::Radians(k_pitch));
    front.z = Math::Sin(Math::Radians(k_yaw)) * Math::Cos(Math::Radians(k_pitch));

    m_front = Math::Normalize(front);
    m_right = Math::Normalize(Math::Cross(front, m_worldUp));
    m_up = Math::Normalize(Math::Cross(m_right, m_front));

    m_view = Math::LookAt(transform.GetWorldPosition(), transform.GetWorldPosition() + m_front, m_up);
    m_projection = Math::Perspective(fov, k_aspect, 0.1f, 100.0f);
}