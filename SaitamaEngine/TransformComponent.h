#pragma once
#include "GameObjectComponent.h"

class TransformComponent : public GameObjectComponent
{
public:
    static const char* g_Name;
    virtual const char* VGetName() const { return g_Name; }

    virtual bool VInit(tinyxml2::XMLElement *pData) override;

    DirectX::SimpleMath::Vector3 GetPosition() const { return m_posVector; }
    DirectX::SimpleMath::Vector3 GetRotation() const { return m_rotVector; }

private:
    DirectX::SimpleMath::Vector3 m_posVector;
    DirectX::SimpleMath::Vector3 m_rotVector;
};

