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
    Vector3 GetRight() const { return m_Props.m_ToWorld.Right(); }

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

class CameraNode : public SceneNode
{
public:
    CameraNode(Matrix const *t, 
                const float angle, 
                const float client_width, 
                const float client_height, 
                const float nearest, 
                const float farthest
                /*, Frustum const &frustum*/
                )
        : SceneNode(INVALID_GAMEOBJECT_ID, WeakBaseRenderComponentPtr(), RenderPass_0, t),
        //m_Frustum(frustum),
        m_bActive(true),
        m_DebugCamera(false),
        m_pTarget(shared_ptr<SceneNode>()),
        m_CamOffsetVector(0.0f, 20.0f, -60.0f, 0.0f)
    {
        m_View = Matrix::Identity;
        m_Projection = DirectX::XMMatrixPerspectiveFovLH(angle, client_width / client_height, nearest, farthest);
    }

    virtual HRESULT VRender(Scene *pScene);
    virtual HRESULT VOnRestore(Scene *pScene) { return S_OK; }
    virtual bool VIsVisible(Scene *pScene) const { return m_bActive; }

    //const Frustum &GetFrustum() { return m_Frustum; }
    void SetTarget(shared_ptr<SceneNode> pTarget)
    {
        m_pTarget = pTarget;
    }
    void ClearTarget() { m_pTarget = shared_ptr<SceneNode>(); }
    shared_ptr<SceneNode> GetTarget() { return m_pTarget; }

    Matrix GetWorldViewProjection(Scene *pScene);
    HRESULT SetViewTransform(Scene *pScene);

    Matrix GetProjection() { return m_Projection; }
    Matrix GetView() { return m_View; }

    void SetCameraOffset(const Vector4 & cameraOffset)
    {
        m_CamOffsetVector = cameraOffset;
    }

protected:

    //Frustum			m_Frustum;
    Matrix m_Projection;
    Matrix m_View;
    bool m_bActive;
    bool m_DebugCamera;
    shared_ptr<SceneNode> m_pTarget;
    Vector4 m_CamOffsetVector;	//Direction of camera relative to target.
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


class D3DGrid : public SceneNode
{
protected:
    DWORD m_numVerts;
    DWORD m_numPolys;

    ID3D11Buffer *m_pIndexBuffer;
    ID3D11Buffer *m_pVertexBuffer;

    VertexShader m_vertexShader;
    PixelShader m_pixelShader;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

    ConstantBuffer<CB_VS_vertexshader> m_constantBuffer;

public:
    bool m_bTextureHasAlpha;

    D3DGrid(GameObjectId gameObjectId, WeakBaseRenderComponentPtr renderComponent, /* const std::string& name, const char* textureResource, int squares, const Color &diffuseColor, */ const Matrix* pMatrix);
    virtual ~D3DGrid();
    virtual HRESULT VOnRestore(Scene *pScene);
    virtual HRESULT VRender(Scene *pScene);
    virtual HRESULT VOnUpdate(Scene *pScene, DWORD const elapsedMs) { return S_OK; }
    //virtual HRESULT VPick(Scene *pScene, RayCast *pRayCast) { return E_FAIL; }

    bool VHasAlpha() const { return m_bTextureHasAlpha; }
};

class SphereNode : public SceneNode
{
public:
    SphereNode(const GameObjectId gameObjectId,
        WeakBaseRenderComponentPtr renderComponent,
        RenderPass renderPass,
        const Matrix *t);

    virtual HRESULT VOnRestore(Scene *pScene);
    virtual HRESULT VOnLostDevice(Scene *pScene) { return S_OK; }
    virtual HRESULT VRender(Scene *pScene);
    //virtual HRESULT VPick(Scene *pScene, RayCast *pRayCast);

protected:
    std::unique_ptr<DirectX::GeometricPrimitive> m_sphere;
    VertexShader m_vertexShader;
    PixelShader m_pixelShader;

    //float CalcBoundingSphere(CDXUTSDKMesh *mesh11);			// this was added post press.
};