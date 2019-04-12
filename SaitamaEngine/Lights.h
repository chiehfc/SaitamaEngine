#pragma once
#include "SceneNode.h"
#include "D3DRenderer11.h"

class LightNode : public SceneNode
{
protected:

public:
    LightNode(const GameObjectId gameObjectId, WeakRenderComponentPtr renderComponent, const Matrix *t);
};

class D3DLightNode11 : public LightNode
{
public:
    D3DLightNode11(const GameObjectId gameObjectId, WeakRenderComponentPtr renderComponent, const Matrix *t)
        : LightNode(gameObjectId, renderComponent, t) { }

    virtual HRESULT VOnRestore() { return S_OK; };
    virtual HRESULT VOnUpdate(Scene *, DWORD const elapsedMs);
};

class LightManager
{
    friend class Scene;

protected:
    Lights m_Lights;
    Vector3 m_vLightDir[8];
    Color m_vLightDiffuse[8];
    Vector3 m_vLightAmbient;
public:
    void CalcLighting(Scene *pScene);
    void CalcLighting(CB_PS_light* pLighting, SceneNode *pNode);
    int GetLightCount(const SceneNode *node) { return m_Lights.size(); }
    const Vector3 *GetLightAmbient(const SceneNode *node) { return &m_vLightAmbient; }
    const Vector3 *GetLightDirection(const SceneNode *node) { return m_vLightDir; }
    const Color *GetLightDiffuse(const SceneNode *node) { return m_vLightDiffuse; }
};
