#pragma once
#include "Geometry.h"
#include "SceneNode.h"
#include <map>
#include "GameObject.h"
#include "MatrixStack.h"
#include "IRenderer.h"

using namespace DirectX::SimpleMath;

// Forward declarations
typedef std::map<GameObjectId, shared_ptr<ISceneNode> > SceneGameObjectMap;

class SceneNode;
//class CameraNode;
//class SkyNode;
//class LightNode;
class LightManager;

class Scene
{
protected:
    shared_ptr<SceneNode> m_Root;
    //shared_ptr<CameraNode> 	m_Camera;
    shared_ptr<IRenderer> m_Renderer;
    
    MatrixStack *m_MatrixStack;
    //AlphaSceneNodes 		m_AlphaSceneNodes;
    SceneGameObjectMap m_GameObjectMap;

    LightManager *m_LightManager;

    //void RenderAlphaPass();

public:
    Scene(shared_ptr<IRenderer> renderer);
    virtual ~Scene();

    HRESULT OnRender();
    HRESULT OnRestore();
    HRESULT OnLostDevice();
    HRESULT OnUpdate(const int deltaMilliseconds);
    shared_ptr<ISceneNode> FindGameObject(GameObjectId id);
    bool AddChild(GameObjectId id, shared_ptr<ISceneNode> kid);
    bool RemoveChild(GameObjectId id);

    // event delegates
    //void NewRenderComponentDelegate(IEventDataPtr pEventData);
    //void ModifiedRenderComponentDelegate(IEventDataPtr pEventData);			// added post-press!
    //void DestroyActorDelegate(IEventDataPtr pEventData);
    //void MoveActorDelegate(IEventDataPtr pEventData);

    //void SetCamera(shared_ptr<CameraNode> camera) { m_Camera = camera; }
    //const shared_ptr<CameraNode> GetCamera() const { return m_Camera; }

    void PushAndSetMatrix(const DirectX::SimpleMath::Matrix &toWorld)
    {
        // Note this code carefully!!!!! It is COMPLETELY different
        // from some DirectX 9 documentation out there....
        // Scene::PushAndSetMatrix - Chapter 16, page 541

        m_MatrixStack->Push();
        m_MatrixStack->PreMult(toWorld);
        DirectX::SimpleMath::Matrix mat = GetTopMatrix();
        m_Renderer->VSetWorldTransform(&mat);
    }

    void PopMatrix()
    {
        // Scene::PopMatrix - Chapter 16, page 541
        m_MatrixStack->Pop();
        Matrix mat = GetTopMatrix();
        m_Renderer->VSetWorldTransform(&mat);
    }

    const Matrix GetTopMatrix()
    {
        // Scene::GetTopMatrix - Chapter 16, page 541
        return static_cast<const Matrix>(*m_MatrixStack->GetTop());
    }

    //LightManager *GetLightManager() { return m_LightManager; }

    //void AddAlphaSceneNode(AlphaSceneNode *asn) { m_AlphaSceneNodes.push_back(asn); }

    //HRESULT Pick(RayCast *pRayCast) { return m_Root->VPick(this, pRayCast); }

    shared_ptr<IRenderer> GetRenderer() { return m_Renderer; }
};

