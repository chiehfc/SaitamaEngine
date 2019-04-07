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