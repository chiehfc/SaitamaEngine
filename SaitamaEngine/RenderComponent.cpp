#include "pch.h"
#include "RenderComponent.h"
#include "TransformComponent.h"

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

shared_ptr<SceneNode> RenderComponent::VCreateSceneNode(void)
{
    // get the transform component
    shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
    if (pTransformComponent)
    {
        WeakRenderComponentPtr weakThis(this);
        //Matrix rot90;
        //rot90.BuildRotationY(-GCC_PI / 2.0f);
        shared_ptr<SceneNode> parent(new SceneNode(m_pOwner->GetId(), weakThis, RenderPass_Actor, &pTransformComponent->GetTransform()));
        shared_ptr<SceneNode> teapot(new D3DTeapotMeshNode11(INVALID_GAMEOBJECT_ID, weakThis, RenderPass_Actor, &pTransformComponent->GetTransform()));
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