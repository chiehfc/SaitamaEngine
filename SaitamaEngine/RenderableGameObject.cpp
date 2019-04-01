#include "pch.h"
#include "RenderableGameObject.h"

using namespace DirectX::SimpleMath;

bool RenderableGameObject::Initialize(const std::string &filePath, ID3D11Device *device, ID3D11DeviceContext *deviceContext, ConstantBuffer<CB_VS_vertexshader> &cb_vs_vertexshader)
{
    if (!m_gameModel.Initialize(filePath, device, deviceContext, cb_vs_vertexshader))
    {
        return false;
    }

    SetPosition(Vector3::Zero);
    SetRotation(Vector3::Zero);
    UpdateMatrix();
    return true;
}

void RenderableGameObject::Draw(const DirectX::XMMATRIX &viewProjectionMatrix)
{
    m_gameModel.Draw(m_worldMatrix, viewProjectionMatrix);
}

void RenderableGameObject::UpdateMatrix()
{
    m_worldMatrix = DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f) * DirectX::XMMatrixRotationRollPitchYaw(
        m_rotVector.x,
        m_rotVector.y,
        m_rotVector.z)
        * DirectX::XMMatrixTranslation(
            m_posVector.x,
            m_posVector.y,
            m_posVector.z);

    Matrix vecRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, m_rotVector.y, 0.0f);
    m_vecForward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    m_vecBack = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
    m_vecLeft = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
    m_vecRight = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}