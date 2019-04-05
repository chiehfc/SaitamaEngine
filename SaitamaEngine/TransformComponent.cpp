#include "pch.h"
#include "TransformComponent.h"


const char* TransformComponent::g_Name = "TransformComponent";

TransformComponent::TransformComponent()
{
}


TransformComponent::~TransformComponent()
{
}

bool TransformComponent::VInit(tinyxml2::XMLElement *pData)
{

    tinyxml2::XMLElement *pPositionElement = pData->FirstChildElement("Position");
    if (pPositionElement)
    {
        double x = 0;
        double y = 0;
        double z = 0;
        pPositionElement->QueryAttribute("x", &x);
        pPositionElement->QueryAttribute("y", &y);
        pPositionElement->QueryAttribute("z", &z);
        m_posVector = DirectX::SimpleMath::Vector3(x, y, z);
    }

    tinyxml2::XMLElement *pOrientationElement = pData->FirstChildElement("YawPitchRoll");
    if (pOrientationElement)
    {
        double yaw = 0;
        double pitch = 0;
        double roll = 0;
        pOrientationElement->QueryAttribute("x", &yaw);
        pOrientationElement->QueryAttribute("y", &pitch);
        pOrientationElement->QueryAttribute("z", &roll);
        m_rotVector = DirectX::SimpleMath::Vector3(yaw, pitch, roll);
    }

    return true;
}
