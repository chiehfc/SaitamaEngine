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

    const DirectX::SimpleMath::Vector3 &GetForwardVector(bool omitY = false);
    const DirectX::SimpleMath::Vector3 &GetLeftVector(bool omitY = false);
    const DirectX::SimpleMath::Vector3 &GetRightVector(bool omitY = false);
    const DirectX::SimpleMath::Vector3 &GetBackwardVector(bool omitY = false);

protected:
    virtual void UpdateMatrix();
    void UpdateDirectionVectors();

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
    
    DirectX::SimpleMath::Vector3 m_vecForward_noY;
    DirectX::SimpleMath::Vector3 m_vecLeft_noY;
    DirectX::SimpleMath::Vector3 m_vecRight_noY;
    DirectX::SimpleMath::Vector3 m_vecBack_noY;
};

