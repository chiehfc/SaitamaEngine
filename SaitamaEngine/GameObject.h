#pragma once
#include "GameModel.h"

class GameObject
{
public:   
    const DirectX::SimpleMath::Vector3 &GetPositionVector() const;
    const DirectX::SimpleMath::Vector3 &GetRotationVector() const;

    void SetPosition(const DirectX::SimpleMath::Vector3 &pos);
    void AdjustPosition(const DirectX::SimpleMath::Vector3 &pos);
    void SetRotation(const DirectX::SimpleMath::Vector3 &rot);
    void AdjustRotation(const DirectX::SimpleMath::Vector3 &rot);
    void SetLookAtPos(const DirectX::SimpleMath::Vector3 &pos);

    const DirectX::SimpleMath::Vector3 &GetForwardVector();
    const DirectX::SimpleMath::Vector3 &GetLeftVector();
    const DirectX::SimpleMath::Vector3 &GetRightVector();
    const DirectX::SimpleMath::Vector3 &GetBackwardVector();

protected:
    virtual void UpdateMatrix();

    DirectX::SimpleMath::Vector3 m_posVector;
    DirectX::SimpleMath::Vector3 m_rotVector;

    const DirectX::SimpleMath::Vector3 DEFAULT_FORWARD_VECTOR = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f);
    const DirectX::SimpleMath::Vector3 DEFAULT_UP_VECTOR = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
    const DirectX::SimpleMath::Vector3 DEFAULT_BACKWARD_VECTOR = DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f);
    const DirectX::SimpleMath::Vector3 DEFAULT_LEFT_VECTOR = DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f);
    const DirectX::SimpleMath::Vector3 DEFAULT_RIGHT_VECTOR = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);

    DirectX::SimpleMath::Vector3 m_vecForward;
    DirectX::SimpleMath::Vector3 m_vecLeft;
    DirectX::SimpleMath::Vector3 m_vecRight;
    DirectX::SimpleMath::Vector3 m_vecBack;
    
};

