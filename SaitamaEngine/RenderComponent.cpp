#include "pch.h"
#include "RenderComponent.h"
#include "TransformComponent.h"

const char *RenderComponent::g_Name = "RenderComponent";
const char* LightRenderComponent::g_Name = "LightRenderComponent";

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

shared_ptr<SceneNode> RenderComponent::VCreateSceneNode(void)
{
    // get the transform component
    shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
    if (pTransformComponent)
    {
        WeakRenderComponentPtr weakThis(this);
        Matrix rot90;
        rot90 = rot90.CreateRotationY(-3.14f / 2.0f);
        shared_ptr<SceneNode> parent(new SceneNode(m_pOwner->GetId(), weakThis, RenderPass_Actor, &pTransformComponent->GetTransform()));
        shared_ptr<SceneNode> teapot(new GameModelNode(INVALID_GAMEOBJECT_ID, weakThis, m_filePath, RenderPass_Actor, &rot90));
        parent->VAddChild(teapot);
        return parent;        
    }

    return shared_ptr<SceneNode>();
}

shared_ptr<SceneNode> RenderComponent::VGetSceneNode(void)
{
    if (!m_pSceneNode)
        m_pSceneNode = VCreateSceneNode();
    return m_pSceneNode;
}

bool LightRenderComponent::VInit(tinyxml2::XMLElement *pData)
{
    tinyxml2::XMLElement *pNode = pData->FirstChildElement("Color");

    if (pNode)
    {
        m_lightProps.color = Color(pNode->FloatAttribute("r"), pNode->FloatAttribute("g"), pNode->FloatAttribute("b"), pNode->FloatAttribute("a"));
    }

    tinyxml2::XMLElement *pLight = pData->FirstChildElement("Light");
    if (pLight)
    {
        tinyxml2::XMLElement *pStrength = pLight->FirstChildElement("LightStrength");
        if (pStrength)
        {
            m_lightProps.lightStrength = pStrength->FloatAttribute("strength");
        }        
        tinyxml2::XMLElement *pAttenuation = pLight->FirstChildElement("Attenuation");
        if (pAttenuation)
        {
            m_lightProps.attenuation[0] = pAttenuation->FloatAttribute("const");
            m_lightProps.attenuation[1] = pAttenuation->FloatAttribute("linear");
            m_lightProps.attenuation[2] = pAttenuation->FloatAttribute("exp");
        }
    }

    return true;
}

//
//bool LightRenderComponent::VDelegateInit(TiXmlElement* pData)
//{
//    TiXmlElement* pLight = pData->FirstChildElement("Light");
//
//    double temp;
//    TiXmlElement* pAttenuationNode = NULL;
//    pAttenuationNode = pLight->FirstChildElement("Attenuation");
//    if (pAttenuationNode)
//    {
//        double temp;
//        pAttenuationNode->Attribute("const", &temp);
//        m_Props.m_Attenuation[0] = (float)temp;
//
//        pAttenuationNode->Attribute("linear", &temp);
//        m_Props.m_Attenuation[1] = (float)temp;
//
//        pAttenuationNode->Attribute("exp", &temp);
//        m_Props.m_Attenuation[2] = (float)temp;
//    }
//
//    TiXmlElement* pShapeNode = NULL;
//    pShapeNode = pLight->FirstChildElement("Shape");
//    if (pShapeNode)
//    {
//        pShapeNode->Attribute("range", &temp);
//        m_Props.m_Range = (float)temp;
//        pShapeNode->Attribute("falloff", &temp);
//        m_Props.m_Falloff = (float)temp;
//        pShapeNode->Attribute("theta", &temp);
//        m_Props.m_Theta = (float)temp;
//        pShapeNode->Attribute("phi", &temp);
//        m_Props.m_Phi = (float)temp;
//    }
//    return true;
//}

shared_ptr<SceneNode> LightRenderComponent::VCreateSceneNode(void)
{
    shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
    if (pTransformComponent)
    {
        WeakRenderComponentPtr weakThis(this);

        return shared_ptr<SceneNode>(new D3DLightNode11(m_pOwner->GetId(), weakThis, m_lightProps, &(pTransformComponent->GetTransform())));

    }
    return shared_ptr<SceneNode>();
}