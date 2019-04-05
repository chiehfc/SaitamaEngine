#pragma once
#include "GameObjectComponent.h"

class TransformComponent : public GameObjectComponent
{
public:
    static const char* g_Name;
    virtual const char* VGetName() const { return g_Name; }

    virtual bool VInit(tinyxml2::XMLElement *pData) override;

    TransformComponent();
    ~TransformComponent();
private:
    DirectX::SimpleMath::Vector3 m_posVector;
    DirectX::SimpleMath::Vector3 m_rotVector;
};

