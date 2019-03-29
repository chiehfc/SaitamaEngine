#pragma once
#include "Math.h"

class Camera
{
  public:
    Camera::Camera();
    
    Camera::~Camera();
    Camera(const Camera& camera);

    Camera& operator=(const Camera& camera);

    void InitProjMatrix(const float angle, const float client_width, const float client_height, const float nearest, const float farthest);
    void InitOrthoMatrix(const float client_width, const float client_height, const float near_plane, const float far_plane);
    void OnResize(uint32_t new_width, uint32_t new_height);

    void Move(DirectX::SimpleMath::Vector3 direction);

    void Walk(float d);

    void Strafe(float d);

    void Pitch(float angle);

    void RotateY(float angle);

    void Rotate(DirectX::SimpleMath::Vector3 axis, float degrees);

    void SetPosition(DirectX::SimpleMath::Vector3& new_position);

    const DirectX::SimpleMath::Vector3& GetPosition() const { return m_position; }

    void SetTarget(DirectX::SimpleMath::Vector3 new_target);

    const DirectX::SimpleMath::Vector3& GetTarget() const { return m_target; }
    const DirectX::SimpleMath::Vector3 GetUp() {
      DirectX::SimpleMath::Vector3 v;
      (m_up - m_position).Normalize(v); return v;
    }

    const DirectX::SimpleMath::Vector3 GetLookAtTarget() {
      DirectX::SimpleMath::Vector3 v;
      (m_target - m_position).Normalize(v); return v;
    }

    // Get transposed camera view matrix
    const DirectX::SimpleMath::Matrix GetView() {
      //DirectX::SimpleMath::Matrix m;
      //m_view.Transpose(m);
      //return m;
      return m_view;
    }

    void SetAngle(float angle);

    const float& GetAngle() const { return m_angle; }

    void SetNearestPlane(float nearest);

    void SetFarthestPlane(float farthest);

    const DirectX::SimpleMath::Matrix Proj() {
      //DirectX::SimpleMath::Matrix m;
      //m_proj.Transpose(m);
      //return m;
      return m_proj;
    }

    const DirectX::SimpleMath::Matrix Ortho() {
      DirectX::SimpleMath::Matrix m;
      m_ortho.Transpose(m);
      return m;
    }

    void UpdateViewMatrix();

  private:
    void InitViewMatrix();

    // Camera parameters
    DirectX::SimpleMath::Vector3 m_position;
    DirectX::SimpleMath::Vector3 m_right;
    DirectX::SimpleMath::Vector3 m_target;
    DirectX::SimpleMath::Vector3 m_up;
    

    // Projection parameters
    float m_angle;
    float m_clientWidth;
    float m_clientHeight;
    float m_nearest;
    float m_farthest;

    DirectX::SimpleMath::Matrix m_view;
    DirectX::SimpleMath::Matrix m_proj;
    DirectX::SimpleMath::Matrix m_ortho;
  
};