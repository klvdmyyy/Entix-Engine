#include "Scene/Components.h"

#include "Math/MatrixTransform.h"
#include "Math/Quat.h"

namespace ERUNTIME_NAMESPACE {
    Float4x4 TransformComponent::GetLocalMatrix() const noexcept
    {
        Float4x4 rot = Math::ToFloat4x4(Quat(rotation));

        return Math::Translate(Float4x4(1.0f), position)
            * rot * Math::Scale(Float4x4(1.0f), scale);
    }

    Float4x4 TransformComponent::GetWorldMatrix(const Float4x4& parentWorld) const noexcept
    {
        return parentWorld * GetLocalMatrix();
    }

    void CameraComponent::Update(const TransformComponent& transform, float aspect)
    {
        m_yaw = YAW + transform.rotation.x;
        m_pitch = PITCH + transform.rotation.y;

        Float3 front;
        front.x = Math::Cos(Math::Radians(m_yaw)) * Math::Cos(Math::Radians(m_pitch));
        front.y = Math::Sin(Math::Radians(m_pitch));
        front.z = Math::Sin(Math::Radians(m_yaw)) * Math::Cos(Math::Radians(m_pitch));

        m_front = Math::Normalize(front);
        m_right = Math::Normalize(Math::Cross(front, m_worldUp));
        m_up = Math::Normalize(Math::Cross(m_right, m_front));

        m_view = Math::LookAt(transform.position, transform.position + m_front, m_up);
        m_projection = Math::Perspective(fov, aspect, 0.1f, 100.0f);
    }
}