#include "pch.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "Events.h"

const char *ModelRenderComponent::g_Name = "ModelRenderComponent";
const char *LightRenderComponent::g_Name = "LightRenderComponent";
const char *GridRenderComponent::g_Name = "GridRenderComponent";

bool BaseRenderComponent::VInit(tinyxml2::XMLElement *pData)
{
    return VDelegateInit(pData);
}

void BaseRenderComponent::VPostInit(void)
{
    shared_ptr<SceneNode> pSceneNode(VGetSceneNode());
    shared_ptr<EvtData_New_Render_Component> pEvent(new EvtData_New_Render_Component(m_pOwner->GetId(), pSceneNode));
    IEventManager::Get()->VTriggerEvent(pEvent);
}

shared_ptr<SceneNode> BaseRenderComponent::VGetSceneNode(void)
{
    if (!m_pSceneNode)
        m_pSceneNode = VCreateSceneNode();
    return m_pSceneNode;
}

bool ModelRenderComponent::VDelegateInit(tinyxml2::XMLElement *pData)
{
    tinyxml2::XMLElement *pNode = pData->FirstChildElement("FilePath");

    if (pNode)
    {        
        m_filePath = pNode->Attribute("filepath");
    }

    return true;
}

shared_ptr<SceneNode> ModelRenderComponent::VCreateSceneNode(void)
{
    // get the transform component
    shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
    if (pTransformComponent)
    {
        WeakBaseRenderComponentPtr weakThis(this);
        Matrix rot90;
        //rot90 = rot90.CreateRotationY(-3.14f / 2.0f);
        shared_ptr<SceneNode> parent(new SceneNode(m_pOwner->GetId(), weakThis, RenderPass_Actor, &pTransformComponent->GetTransform()));
        shared_ptr<SceneNode> teapot(new GameModelNode(INVALID_GAMEOBJECT_ID, weakThis, m_filePath, RenderPass_Actor, &rot90));
        parent->VAddChild(teapot);
        return parent;        
    }

    return shared_ptr<SceneNode>();
}

bool LightRenderComponent::VDelegateInit(tinyxml2::XMLElement *pData)
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

shared_ptr<SceneNode> LightRenderComponent::VCreateSceneNode(void)
{
    shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
    if (pTransformComponent)
    {
        WeakBaseRenderComponentPtr weakThis(this);

        return shared_ptr<SceneNode>(new D3DLightNode11(m_pOwner->GetId(), weakThis, m_lightProps, &(pTransformComponent->GetTransform())));

    }
    return shared_ptr<SceneNode>();
}




bool GridRenderComponent::VDelegateInit(tinyxml2::XMLElement *pData)
{
    tinyxml2::XMLElement *pTexture = pData->FirstChildElement("Texture");
    if (pTexture)
    {
        m_textureResource = pTexture->FirstChild()->Value();
    }

    tinyxml2::XMLElement *pDivision = pData->FirstChildElement("Division");
    if (pDivision)
    {
        m_squares = atoi(pDivision->FirstChild()->Value());
    }

    return true;
}

shared_ptr<SceneNode> GridRenderComponent::VCreateSceneNode(void)
{
    shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
    if (pTransformComponent)
    {
        WeakBaseRenderComponentPtr weakThis(this);
        return shared_ptr<SceneNode>(new D3DGrid(m_pOwner->GetId(), weakThis, &(pTransformComponent->GetTransform())));
    }

    return shared_ptr<SceneNode>();
}