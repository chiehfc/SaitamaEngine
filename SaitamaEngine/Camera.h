#pragma once
//#include "Math.h"
#include "GameObject.h"

class Camera : public GameObject
{
  public:
    Camera::Camera();    

    void InitProjMatrix(const float angle, const float client_width, const float client_height, const float nearest, const float farthest);

    const Matrix &GetViewMatrix() const;
    const Matrix &GetProjectionMatrix() const;

    void UpdateMatrix() override;
        
  private:
        
    // Projection parameters
    float m_angle;
    float m_clientWidth;
    float m_clientHeight;
    float m_nearest;
    float m_farthest;

    DirectX::SimpleMath::Matrix m_view;
    DirectX::SimpleMath::Matrix m_proj;
};