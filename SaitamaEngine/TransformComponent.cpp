#include "pch.h"
#include "TransformComponent.h"

const char* TransformComponent::g_Name = "TransformComponent";

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

    tinyxml2::XMLElement *pScaleElement = pData->FirstChildElement("Scale");
    if (pScaleElement)
    {
        double x = 0;
        double y = 0;
        double z = 0;
        pScaleElement->QueryAttribute("x", &x);
        pScaleElement->QueryAttribute("y", &y);
        pScaleElement->QueryAttribute("z", &z);
        m_scale = DirectX::SimpleMath::Vector3((float)x, (float)y, (float)z);
    }

    DirectX::SimpleMath::Matrix translation;
    translation = translation.CreateTranslation(m_posVector);
    
    DirectX::SimpleMath::Matrix rotation;
    rotation = rotation.CreateFromYawPitchRoll(DirectX::XMConvertToRadians(m_rotVector.x), DirectX::XMConvertToRadians(m_rotVector.y), DirectX::XMConvertToRadians(m_rotVector.z));

    DirectX::SimpleMath::Matrix scale;
    scale = scale.CreateScale(m_scale);

    m_transform = scale * rotation * translation;

    /**
    // This is not supported yet.
    TiXmlElement* pLookAtElement = pData->FirstChildElement("LookAt");
    if (pLookAtElement)
    {
        double x = 0;
        double y = 0;
        double z = 0;
        pLookAtElement->Attribute("x", &x);
        pLookAtElement->Attribute("y", &y);
        pLookAtElement->Attribute("z", &z);

        Vec3 lookAt((float)x, (float)y, (float)z);
        rotation.BuildRotationLookAt(translation.GetPosition(), lookAt, g_Up);
    }

    
    **/

    

    return true;
}
