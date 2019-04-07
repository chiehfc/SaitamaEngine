#include "pch.h"
#include "RenderComponent.h"

const char *RenderComponent::g_Name = "RenderComponent";

bool RenderComponent::VInit(tinyxml2::XMLElement *pData)
{
    tinyxml2::XMLElement *pNode = pData->FirstChildElement("FilePath");

    if (pNode)
    {        
        m_filePath = pNode->Attribute("filepath");
    }

    return true;
}

void RenderComponent::UpdateMatrix() 
{
    /*m_worldMatrix = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) * DirectX::XMMatrixRotationRollPitchYaw(
        m_rotVector.x,
        m_rotVector.y,
        m_rotVector.z)
        * DirectX::XMMatrixTranslation(
            m_posVector.x,
            m_posVector.y,
            m_posVector.z);

    UpdateDirectionVectors();*/
}