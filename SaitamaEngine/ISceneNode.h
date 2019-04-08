#pragma once

#include "pch.h"
#include "GameObject.h"

class SceneNodeProperties;
class Scene;

class ISceneNode
{
public:
    virtual const SceneNodeProperties* VGet() const = 0;

    virtual void VSetTransform(const DirectX::SimpleMath::Matrix *toWorld, const DirectX::SimpleMath::Matrix *fromWorld = NULL) = 0;

    virtual HRESULT VOnUpdate(Scene *pScene, DWORD const elapsedMs) = 0;
    virtual HRESULT VOnRestore(Scene *pScene) = 0;

    virtual HRESULT VPreRender(Scene *pScene) = 0;
    //virtual bool VIsVisible(Scene *pScene) const = 0;
    virtual HRESULT VRender(Scene *pScene) = 0;
    virtual HRESULT VRenderChildren(Scene *pScene) = 0;
    virtual HRESULT VPostRender(Scene *pScene) = 0;

    virtual bool VAddChild(shared_ptr<ISceneNode> kid) = 0;
    virtual bool VRemoveChild(GameObjectId id) = 0;
    virtual HRESULT VOnLostDevice(Scene *pScene) = 0;
    //virtual HRESULT VPick(Scene *pScene, RayCast *pRayCast) = 0;


    virtual ~ISceneNode() { };
};
