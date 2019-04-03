#include "pch.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;

Camera::Camera() {
  m_posVector = Vector3(0.0f, 50.0f, -100.0f);
  m_angle = DirectX::XM_PI / 4.f;
  m_clientWidth = 800.0f;
  m_clientHeight = 600.0f;
  m_nearest = 0.1f;
  m_farthest = 1000.0f;
  m_view = Matrix::Identity;
  m_proj = Matrix::Identity;
  InitProjMatrix(m_angle, m_clientWidth, m_clientHeight, m_nearest, m_farthest);
}

void Camera::InitProjMatrix(const float angle, const float client_width, const float client_height, const float nearest, const float farthest)
{
  m_angle = angle;
  m_clientWidth = client_width;
  m_clientHeight = client_height;
  m_nearest = nearest;
  m_farthest = farthest;

  m_proj = DirectX::XMMatrixPerspectiveFovLH(angle, m_clientWidth / client_height, nearest, farthest);  
}

void Camera::UpdateMatrix() //Updates view matrix and also updates the movement vectors
{
    //Calculate camera rotation matrix
    Matrix camRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_rotVector.x, m_rotVector.y, m_rotVector.z);
    //Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
    Vector3 camTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, camRotationMatrix);
    //Adjust cam target to be offset by the camera's current position
    camTarget += m_posVector;
    //Calculate up direction based on current rotation
    Vector3 upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
    //Rebuild view matrix
    m_view = XMMatrixLookAtLH(m_posVector, camTarget, upDir);

    UpdateDirectionVectors();
}

const Matrix &Camera::GetViewMatrix() const
{
    return m_view;
}

const Matrix &Camera::GetProjectionMatrix() const
{
    return m_proj;
}