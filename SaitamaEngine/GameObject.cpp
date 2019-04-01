#include "pch.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;


void GameObject::UpdateMatrix()
{
    assert("UpdateMatrix mush be overridden." && 0);
}

const Vector3 &GameObject::GetPositionVector() const
{
    return m_posVector;
}
const Vector3 &GameObject::GetRotationVector() const
{
    return m_rotVector;
}

void GameObject::SetPosition(const Vector3 &pos)
{
    m_posVector = pos;
    UpdateMatrix();
}

void GameObject::AdjustPosition(const Vector3 &pos)
{
    m_posVector += pos;
    UpdateMatrix();
}

void GameObject::SetRotation(const Vector3 &rot)
{
    m_rotVector = rot;
    UpdateMatrix();
}

void GameObject::AdjustRotation(const Vector3 &rot)
{
    m_rotVector += rot;
    UpdateMatrix();
}

void GameObject::SetLookAtPos(const Vector3 &lookAtPos)
{
    //Verify that look at pos is not the same as cam pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior.
    if (lookAtPos == m_posVector) return;

    Vector3 newLookAtPos(m_posVector.x - lookAtPos.x, m_posVector.y - lookAtPos.y, m_posVector.z - lookAtPos.z);

    float pitch = 0.0f;
    if (newLookAtPos.y != 0.0f)
    {
        const float distance = sqrt(newLookAtPos.x * newLookAtPos.x + newLookAtPos.z * newLookAtPos.z);
        pitch = atan(newLookAtPos.y / distance);
    }

    float yaw = 0.0f;
    if (newLookAtPos.x != 0.0f)
    {
        yaw = atan(newLookAtPos.x / newLookAtPos.z);
    }
    if (newLookAtPos.z > 0)
        yaw += DirectX::XM_PI;

    SetRotation(Vector3(pitch, yaw, 0.0f));
}


const Vector3 &GameObject::GetForwardVector()
{
    return m_vecForward;
}

const Vector3 &GameObject::GetLeftVector()
{
    return m_vecLeft;
}

const Vector3 &GameObject::GetRightVector()
{
    return m_vecRight;
}

const Vector3 &GameObject::GetBackwardVector()
{
    return m_vecBack;
}