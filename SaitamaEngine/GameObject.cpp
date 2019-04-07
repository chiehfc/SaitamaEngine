#include "pch.h"
#include "GameObject.h"
#include "GameObjectComponent.h"

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


const Vector3 &GameObject::GetForwardVector(bool omitY)
{
    return omitY ? m_vecForward_noY : m_vecForward;
}

const Vector3 &GameObject::GetLeftVector(bool omitY)
{
    return omitY ? m_vecLeft_noY : m_vecLeft;
}

const Vector3 &GameObject::GetRightVector(bool omitY)
{
    return omitY ? m_vecRight_noY : m_vecRight;
}

const Vector3 &GameObject::GetBackwardVector(bool omitY)
{
    return omitY ? m_vecBack_noY : m_vecBack;
}

void GameObject::UpdateDirectionVectors()
{
    Matrix vecRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_rotVector.x, m_rotVector.y, 0.0f);
    m_vecForward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    m_vecBack = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
    m_vecLeft = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
    m_vecRight = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);

    Matrix vecRotationMatrixNoY = DirectX::XMMatrixRotationRollPitchYaw(0.0f, m_rotVector.y, 0.0f);
    m_vecForward_noY = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrixNoY);
    m_vecBack_noY = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrixNoY);
    m_vecLeft_noY = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrixNoY);
    m_vecRight_noY = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrixNoY);
}



//////////////////// Component System


GameObject::GameObject(GameObjectId id)
{
    m_id = id;
    m_type = "Unknown";
}

GameObject::~GameObject()
{

}

bool GameObject::Init(tinyxml2::XMLElement* pData)
{
    //GCC_LOG("Actor", std::string("Initializing Actor ") + ToStr(m_id));

    m_type = pData->Attribute("type");
    return true;
}

void GameObject::PostInit(void)
{
    for (GameObjectComponents::iterator it = m_components.begin(); it != m_components.end(); ++it)
    {
        it->second->VPostInit();
    }
}

void GameObject::Destroy(void)
{
    m_components.clear();
}

void GameObject::AddComponent(StrongGameObjectComponentPtr pComponent)
{
    std::pair<GameObjectComponents::iterator, bool> success = m_components.insert(std::make_pair(pComponent->VGetId(), pComponent));
}