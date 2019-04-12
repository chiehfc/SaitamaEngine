#include "pch.h"
#include "Lights.h"
#include "RenderComponent.h"
#include "Scene.h"

LightNode::LightNode(const GameObjectId gameObjectId, WeakRenderComponentPtr renderComponent, const Matrix *t)
    : SceneNode(gameObjectId, renderComponent, RenderPass_NotRendered, t)
{
}

HRESULT D3DLightNode11::VOnUpdate(Scene *, DWORD const elapsedMs)
{
    // light color can change anytime! Check the BaseRenderComponent!
    LightRenderComponent* lrc = static_cast<LightRenderComponent*>(m_RenderComponent);    
    return S_OK;
}


//
// LightManager::CalcLighting					- Chapter 16, page 554
//
void LightManager::CalcLighting(Scene *pScene)
{
    // FUTURE WORK: There might be all kinds of things you'd want to do here for optimization, especially turning off lights on actors that can't be seen, etc.
    pScene->GetRenderer()->VCalcLighting(&m_Lights, 8);

    int count = 0;

    //for (Lights::iterator i = m_Lights.begin(); i != m_Lights.end(); ++i, ++count)
    //{
    //    shared_ptr<LightNode> light = *i;

    //    if (count == 0)
    //    {
    //        // TODO: Set Ambient light in XML.
    //        // Light 0 is the only one we use for ambient lighting. The rest are ignored in the simple shaders used for GameCode4.
    //        //Color ambient = light->VGet()->GetMaterial().GetAmbient();
    //        m_vLightAmbient = Vector3(1.0f, 1.0f, 1.0f);
    //    }

    //    Vector3 lightDir = light->GetDirection();
    //    m_vLightDir[count] = Vector3(lightDir.x, lightDir.y, lightDir.z);
    //    //m_vLightDiffuse[count] = light->VGet()->GetMaterial().GetDiffuse();
    //    m_vLightDiffuse[count] = Color(1.0f, 1.0f, 1.0f);
    //}
}


void LightManager::CalcLighting(CB_PS_light* pLighting, SceneNode *pNode)
{
    int count = GetLightCount(pNode);
    if (count)
    {
        pLighting->ambientLightColor = *GetLightAmbient(pNode);
        memcpy(pLighting->dynamicLightPosition, GetLightDirection(pNode), sizeof(Vector3) * count);
        memcpy(pLighting->dynamicLightColor, GetLightDiffuse(pNode), sizeof(Vector3) * count);
        pLighting->numberOfLights = count;
    }
}

