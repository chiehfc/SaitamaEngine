#include "pch.h"
#include "Camera.h"


using namespace DirectX::SimpleMath;

Camera::Camera() {
  m_position = Vector3(0.0f, 0.0f, 0.0f);
  m_right = Vector3(1.0f, 0.0f, 0.0f);
  m_target = Vector3(0.0f, 0.0f, 1.0f);
  m_up = Vector3::UnitY;
  //this->InitViewMatrix();
  m_angle = DirectX::XM_PI / 4.f;
  m_clientWidth = 800.0f;
  m_clientHeight = 600.0f;
  m_nearest = 0.1f;
  m_farthest = 10.0f;
  m_view = Matrix::Identity;
  m_proj = Matrix::Identity;
  m_ortho = Matrix::Identity;
  InitProjMatrix(m_angle, m_clientWidth, m_clientHeight, m_nearest, m_farthest);
}

Camera::~Camera() {

}
Camera::Camera(const Camera& camera) {
  *this = camera;
}

Camera& Camera::operator=(const Camera& camera)
{
  m_position = camera.m_position;
  m_right = camera.m_right;
  m_target = camera.m_target;
  m_up = camera.m_up;
  m_angle = camera.m_angle;
  m_clientWidth = camera.m_clientWidth;
  m_clientHeight = camera.m_clientHeight;
  m_nearest = camera.m_nearest;
  m_farthest = camera.m_farthest;
  m_view = camera.m_view;
  m_proj = camera.m_proj;
  m_ortho = camera.m_ortho;
  return *this;
}

void Camera::InitViewMatrix()
{
  m_view = Matrix::CreateLookAt(m_position, m_target, m_up);
}

void Camera::InitProjMatrix(const float angle, const float client_width, const float client_height, const float nearest, const float farthest)
{
  m_angle = angle;
  m_clientWidth = client_width;
  m_clientHeight = client_height;
  m_nearest = nearest;
  m_farthest = farthest;
  m_proj = Matrix::CreatePerspectiveFieldOfView(angle, client_width / client_height, nearest, farthest);
}
void Camera::InitOrthoMatrix(const float client_width, const float client_height, const float near_plane, const float far_plane)
{
  m_clientWidth = client_width;
  m_clientHeight = client_height;
  m_nearest = near_plane;
  m_farthest = far_plane;
  m_ortho = Matrix::CreateOrthographic(client_width, client_height, 0.0f, far_plane);
}
void Camera::OnResize(uint32_t new_width, uint32_t new_height)
{
  m_clientWidth = new_width;
  m_clientHeight = new_height;
  InitProjMatrix(m_angle, static_cast<float>(new_width), static_cast<float>(new_height), m_nearest, m_farthest);
  InitOrthoMatrix(static_cast<float>(new_width), static_cast<float>(new_height), 0.0f, m_farthest);
}

void Camera::Strafe(float d) {
  //m_position = Vector3::Transform(m_position, Matrix::CreateTranslation(direction));
  //m_target = Vector3::Transform(m_target, Matrix::CreateTranslation(direction));
  //m_up = Vector3::Transform(m_up, Matrix::CreateTranslation(direction));
  //this->InitViewMatrix();
  m_position += m_right * d;
}

void Camera::Move(DirectX::SimpleMath::Vector3 direction) {
  m_position = Vector3::Transform(m_position, Matrix::CreateTranslation(direction));
  m_target = Vector3::Transform(m_target, Matrix::CreateTranslation(direction));
  m_up = Vector3::Transform(m_up, Matrix::CreateTranslation(direction));
  this->InitViewMatrix();
}

void Camera::Walk(float d)
{
  // mPosition += d*mLook
  m_position += d * m_target;
  //XMVECTOR s = XMVectorReplicate(d);
  //XMVECTOR l = XMLoadFloat3(&mLook);
  //XMVECTOR p = XMLoadFloat3(&mPosition);
  //XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));
}


void Camera::Rotate(DirectX::SimpleMath::Vector3 axis, float degrees) {
  if (axis==Vector3::Zero || degrees == 0.0f) return;

  Vector3 look_at_target = m_target - m_position;
  Vector3 look_at_up = m_up - m_position;
  look_at_target = Vector3::Transform(look_at_target, Matrix::CreateFromAxisAngle(axis, DirectX::XMConvertToRadians(degrees) ));
  look_at_up = Vector3::Transform(look_at_up, Matrix::CreateFromAxisAngle(axis, DirectX::XMConvertToRadians(degrees)));
  m_target = m_position + look_at_target;
  m_up = m_position + look_at_up;
  this->InitViewMatrix();
}

void Camera::SetPosition(DirectX::SimpleMath::Vector3& new_position) {
  Vector3 move_vector = new_position - m_position;
  Vector3 target = m_target;
  this->Move(move_vector);
  this->SetTarget(target);
}

void Camera::SetTarget(DirectX::SimpleMath::Vector3 new_target) {
  if (new_target == m_position || new_target == m_target) return;

  Vector3 old_look_at_target = m_target - m_position;
  Vector3 new_look_at_target = new_target - m_position;
  float angle = DirectX::XMConvertToDegrees(DirectX::XMVectorGetX(
    DirectX::XMVector3AngleBetweenNormals(DirectX::XMVector3Normalize(old_look_at_target),
      DirectX::XMVector3Normalize(new_look_at_target))));

  if (angle != 0.0f && angle != 360.0f && angle != 180.0f)
  {
    Vector3 axis = old_look_at_target.Cross(new_look_at_target);
    Rotate(axis, angle);
  }
  m_target = new_target;
  this->InitViewMatrix();
}

void Camera::SetAngle(float angle)
{
  m_angle = angle;
  this->InitProjMatrix(m_angle, m_clientWidth, m_clientHeight, m_nearest, m_farthest);
}

void Camera::SetNearestPlane(float nearest)
{
  m_nearest = nearest;
  OnResize(m_clientWidth, m_clientHeight);
}

void Camera::SetFarthestPlane(float farthest)
{
  m_farthest = farthest;
  OnResize(m_clientWidth, m_clientHeight);
}

void Camera::UpdateViewMatrix()
{
  Vector3 r = m_right;
  Vector3 u = m_up;
  Vector3 l = m_target;
  Vector3 p = m_position;

  l.Normalize(l);
  u.Normalize(l.Cross(r));
  r = u.Cross(l);
  
  float x = -p.Dot(r);
  float y = -p.Dot(u);
  float z = -p.Dot(l);

  m_right = r;
  m_up = u;
  m_target = l;

  Matrix m;
  m._11 = m_right.x;
  m._21 = m_right.y;
  m._31 = m_right.z;
  m._41 = x;

  m._12 = m_up.x;
  m._22 = m_up.y;
  m._32 = m_up.z;
  m._42 = y;

  m._13 = m_target.x;
  m._23 = m_target.y;
  m._33 = m_target.z;
  m._43 = z;

  m._14 = 0.0f;
  m._24 = 0.0f;
  m._34 = 0.0f;
  m._44 = 1.0f;

  m_view = m;
}