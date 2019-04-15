#pragma once
#include "ISceneNode.h"
#include "Saitama.h"

#include "Shaders.h"
class BaseRenderComponent;
class TransformComponent;

typedef BaseRenderComponent* WeakBaseRenderComponentPtr;

class SceneNodeProperties
{
    friend class SceneNode;

protected:
    GameObjectId m_GameObjectId;
    std::string	m_Name;
    Matrix m_ToWorld, m_FromWorld;
    float m_Radius;
    RenderPass m_RenderPass;
    //Material				m_Material;
    //AlphaType				m_AlphaType;

    /*void SetAlpha(const float alpha)
    {
        m_AlphaType = AlphaMaterial; m_Material.SetAlpha(alpha);
    }*/

public:
    SceneNodeProperties(void);
    const GameObjectId &GameObjectId() const { return m_GameObjectId; }
    Matrix const &ToWorld() const { return m_ToWorld; }
    Matrix const &FromWorld() const { return m_FromWorld; }
    void Transform(Matrix *toWorld, Matrix *fromWorld) const;

    const char *Name() const { return m_Name.c_str(); }

    /*bool HasAlpha() const { return m_Material.HasAlpha(); }
    float Alpha() const { return m_Material.GetAlpha(); }
    AlphaType AlphaType() const { return m_AlphaType; }
    */

    RenderPass RenderPass() const { return m_RenderPass; }
    //float Radius() const { return m_Radius; }

    /*Material GetMaterial() const { return m_Material; }*/
};


typedef std::vector<shared_ptr<ISceneNode> > SceneNodeList;

class SceneNode : public ISceneNode
{
    friend class Scene;

protected:
    SceneNodeList			m_Children;
    SceneNode				*m_pParent;
    SceneNodeProperties		m_Props;
    WeakBaseRenderComponentPtr	m_RenderComponent;

public:
    SceneNode(GameObjectId gameObjectId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix *to, const Matrix *from = NULL);

    virtual ~SceneNode();

    virtual const SceneNodeProperties* VGet() const { return &m_Props; }

    virtual void VSetTransform(const Matrix *toWorld, const Matrix *fromWorld = NULL);

    virtual HRESULT VOnRestore(Scene *pScene);
    virtual HRESULT VOnUpdate(Scene *, DWORD const elapsedMs);

    virtual HRESULT VPreRender(Scene *pScene);
    //virtual bool VIsVisible(Scene *pScene) const;
    virtual HRESULT VRender(Scene *pScene) { return S_OK; }
    virtual HRESULT VRenderChildren(Scene *pScene);
    virtual HRESULT VPostRender(Scene *pScene);

    virtual bool VAddChild(shared_ptr<ISceneNode> kid);
    virtual bool VRemoveChild(GameObjectId id);
    virtual HRESULT VOnLostDevice(Scene *pScene);
    //virtual HRESULT VPick(Scene *pScene, RayCast *pRayCast);

    //void SetAlpha(float alpha);
    //float GetAlpha() const { return m_Props.Alpha(); }

    Vector3 GetPosition() const { return m_Props.m_ToWorld.Translation(); }
    void SetPosition(const Vector3 &pos) { m_Props.m_ToWorld.Translation(pos); }

    const Vector3 GetWorldPosition() const;					// [mrmike] added post-press to respect ancestor's position 
    
    // TODO: CHECK
    Vector3 GetDirection() const { return m_Props.m_ToWorld.Forward(); }

    //void SetRadius(const float radius) { m_Props.m_Radius = radius; }
    //void SetMaterial(const Material &mat) { m_Props.m_Material = mat; }
};




class RootNode : public SceneNode
{
public:
    RootNode();
    virtual bool VAddChild(shared_ptr<ISceneNode> kid);
    virtual HRESULT VRenderChildren(Scene *pScene);
    virtual bool VRemoveChild(GameObjectId id);
    //virtual bool VIsVisible(Scene *pScene) const { return true; }
};


class GameModelNode : public SceneNode
{
public:
    GameModelNode(const GameObjectId actorId,
        WeakBaseRenderComponentPtr renderComponent,
        std::string sdkMeshFileName,
        RenderPass renderPass,
        const Matrix *t);

    virtual HRESULT VOnRestore(Scene *pScene);
    virtual HRESULT VOnLostDevice(Scene *pScene) { return S_OK; }
    virtual HRESULT VRender(Scene *pScene);
    //virtual HRESULT VPick(Scene *pScene, RayCast *pRayCast);

protected:

    GameModel m_model;

    ConstantBuffer<CB_VS_vertexshader> m_constantBuffer;
    VertexShader m_vertexShader;
    PixelShader m_pixelShader;

    //float CalcBoundingSphere(CDXUTSDKMesh *mesh11);			// this was added post press.
};