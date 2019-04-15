#include "pch.h"
#include "SceneNode.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "Scene.h"
#include "D3DRenderer11.h"

SceneNodeProperties::SceneNodeProperties(void)
{
    m_GameObjectId = INVALID_GAMEOBJECT_ID;
    m_Radius = 0;
    m_RenderPass = RenderPass_0;
    //m_AlphaType = AlphaOpaque;
}


//
// SceneNodeProperties::Transform			- Chapter 16, page 528
//
void SceneNodeProperties::Transform(Matrix *toWorld, Matrix *fromWorld) const
{
    if (toWorld)
        *toWorld = m_ToWorld;

    if (fromWorld)
        *fromWorld = m_FromWorld;
}

SceneNode::SceneNode(GameObjectId gameObjectId, WeakRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix *to, const Matrix *from)
{
    m_pParent = NULL;
    m_Props.m_GameObjectId = gameObjectId;
    m_Props.m_Name = (renderComponent) ? renderComponent->VGetName() : "SceneNode";
    //m_Props.m_RenderPass = renderPass;
    //m_Props.m_AlphaType = AlphaOpaque;
    m_RenderComponent = renderComponent;
    VSetTransform(to, from);
    //SetRadius(0);

    // [mrmike] - these lines were moved to VOnRestore() post press
    //Color color = (renderComponent) ? renderComponent->GetColor() : g_White;
    //m_Props.m_Material.SetDiffuse(color);
}


SceneNode::~SceneNode()
{
}

//
// SceneNode::VOnRestore					- Chapter 16, page 532
//
HRESULT SceneNode::VOnRestore(Scene *pScene)
{
    //Color color = (m_RenderComponent) ? m_RenderComponent->GetColor() : g_White;
    //m_Props.m_Material.SetDiffuse(color);

    // This is meant to be called from any class
    // that inherits from SceneNode and overloads
    // VOnRestore()

    SceneNodeList::iterator i = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();
    while (i != end)
    {
        (*i)->VOnRestore(pScene);
        ++i;
    }
    return S_OK;
}


//
// SceneNode::VOnLostDevice					- Chapter X, page Y
//
HRESULT SceneNode::VOnLostDevice(Scene *pScene)
{
    // This is meant to be called from any class
    // that inherits from SceneNode and overloads
    // VOnRestore()

    SceneNodeList::iterator i = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();
    while (i != end)
    {
        (*i)->VOnLostDevice(pScene);
        ++i;
    }
    return S_OK;
}

//
// SceneNode::VSetTransform					- Chapter 16, page 531
//
void SceneNode::VSetTransform(const Matrix *toWorld, const Matrix *fromWorld)
{
    m_Props.m_ToWorld = *toWorld;
    if (!fromWorld)
    {
        m_Props.m_FromWorld = m_Props.m_ToWorld.Invert();
    }
    else
    {
        m_Props.m_FromWorld = *fromWorld;
    }
}

//
// SceneNode::VPreRender					- Chapter 16, page 532
//
HRESULT SceneNode::VPreRender(Scene *pScene)
{
    // TODO:
    // This was added post press! Is is always ok to read directly from the game logic.
    //StrongGameObjectPtr pActor = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(m_Props.m_ActorId));
    StrongGameObjectPtr pActor;
    if (pActor)
    {
        shared_ptr<TransformComponent> pTc = MakeStrongPtr(pActor->GetComponent<TransformComponent>(TransformComponent::g_Name));
        if (pTc)
        {
            m_Props.m_ToWorld = pTc->GetTransform();
        }
    }

    pScene->PushAndSetMatrix(m_Props.m_ToWorld);
    return S_OK;
}

//
// SceneNode::VPostRender					- Chapter 16, page 532
//
HRESULT SceneNode::VPostRender(Scene *pScene)
{
    pScene->PopMatrix();
    return S_OK;
}



//
// SceneNode::VIsVisible				- Chapter 16, page 533
//	
//bool SceneNode::VIsVisible(Scene *pScene) const
//{
//    // transform the location of this node into the camera space 
//    // of the camera attached to the scene
//
//    Matrix toWorld, fromWorld;
//    pScene->GetCamera()->VGet()->Transform(&toWorld, &fromWorld);
//
//    Vector3 pos = GetWorldPosition();
//
//    Vector3 fromWorldPos = fromWorld.Xform(pos);
//
//    Frustum const &frustum = pScene->GetCamera()->GetFrustum();
//
//    bool isVisible = frustum.Inside(fromWorldPos, VGet()->Radius());
//    return isVisible;
//}

//
// SceneNode::GetWorldPosition			- not described in the book
//
//   This was added post press to respect any SceneNode ancestors - you have to add all 
//   their positions together to get the world position of any SceneNode.
//
const Vector3 SceneNode::GetWorldPosition() const
{
    Vector3 pos = GetPosition();
    if (m_pParent)
    {
        pos += m_pParent->GetWorldPosition();
    }
    return pos;
}


//
// SceneNode::VOnUpdate					- Chapter 16, page 532
//
HRESULT SceneNode::VOnUpdate(Scene *pScene, DWORD const elapsedMs)
{
    // This is meant to be called from any class
    // that inherits from SceneNode and overloads
    // VOnUpdate()

    SceneNodeList::iterator i = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();

    while (i != end)
    {
        (*i)->VOnUpdate(pScene, elapsedMs);
        ++i;
    }
    return S_OK;
}

//
// SceneNode::VRenderChildren					- Chapter 16, page 533
//
HRESULT SceneNode::VRenderChildren(Scene *pScene)
{
    // Iterate through the children....
    SceneNodeList::iterator i = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();

    while (i != end)
    {
        if ((*i)->VPreRender(pScene) == S_OK)
        {
            // You could short-circuit rendering
            // if an object returns E_FAIL from
            // VPreRender()

            (*i)->VRender(pScene);

            (*i)->VRenderChildren(pScene);

            //// Don't render this node if you can't see it
            //if ((*i)->VIsVisible(pScene))
            //{
            //    float alpha = (*i)->VGet()->m_Material.GetAlpha();
            //    if (alpha == fOPAQUE)
            //    {
            //        (*i)->VRender(pScene);
            //    }
            //    else if (alpha != fTRANSPARENT)
            //    {
            //        // The object isn't totally transparent...
            //        AlphaSceneNode *asn = GCC_NEW AlphaSceneNode;
            //        GCC_ASSERT(asn);
            //        asn->m_pNode = *i;
            //        asn->m_Concat = pScene->GetTopMatrix();

            //        Vec4 worldPos(asn->m_Concat.GetPosition());

            //        Mat4x4 fromWorld = pScene->GetCamera()->VGet()->FromWorld();

            //        Vec4 screenPos = fromWorld.Xform(worldPos);

            //        asn->m_ScreenZ = screenPos.z;

            //        pScene->AddAlphaSceneNode(asn);
            //    }

            //    // [mrmike] see comment just below...
            //    (*i)->VRenderChildren(pScene);
            //}

            // [mrmike] post-press fix - if the parent is not visible, the childrend
            //           shouldn't be visible either.
            //(*i)->VRenderChildren(pScene);
        }
        (*i)->VPostRender(pScene);
        ++i;
    }

    return S_OK;
}


//
// SceneNode::VAddChild					- Chapter 16, page 535
//
bool SceneNode::VAddChild(shared_ptr<ISceneNode> ikid)
{
    m_Children.push_back(ikid);

    shared_ptr<SceneNode> kid = static_pointer_cast<SceneNode>(ikid);

    kid->m_pParent = this;					// [mrmike] Post-press fix - the parent was never set!

                                            // The radius of the sphere should be fixed right here
    Vector3 kidPos = kid->VGet()->ToWorld().Translation();

    // [mrmike] - Post-press fix. This was not correct! subtracting the parents's position from the kidPos
    //            created a HUGE radius, depending on the location of the parent, which could be anywhere
    //            in the game world.

    //Vec3 dir = kidPos - m_Props.ToWorld().GetPosition();
    //float newRadius = dir.Length() + kid->VGet()->Radius();

    //float newRadius = kidPos.Length() + kid->VGet()->Radius();

    //if (newRadius > m_Props.m_Radius)
    //    m_Props.m_Radius = newRadius;

    return true;
}

//
// SceneNode::VRemoveChild					- not in the book
//
//   If an actor is destroyed it should be removed from the scene graph.
//   Generally the HumanView will recieve a message saying the actor has been
//   destroyed, and it will then call Scene::RemoveChild which will traverse 
//   the scene graph to find the child that needs removing.
//
bool SceneNode::VRemoveChild(GameObjectId id)
{
    for (SceneNodeList::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
    {
        const SceneNodeProperties* pProps = (*i)->VGet();
        if (pProps->GameObjectId() != INVALID_GAMEOBJECT_ID && id == pProps->GameObjectId())
        {
            i = m_Children.erase(i);	//this can be expensive for vectors
            return true;
        }
    }
    return false;
}


//
//HRESULT SceneNode::VPick(Scene *pScene, RayCast *raycast)
//{
//    for (SceneNodeList::const_iterator i = m_Children.begin(); i != m_Children.end(); ++i)
//    {
//        HRESULT hr = (*i)->VPick(pScene, raycast);
//
//        if (hr == E_FAIL)
//            return E_FAIL;
//    }
//
//    return S_OK;
//}

// This was changed post press - it was convenient to be able to set alpha on a parent node and
// have it propagate to all its children, since materials are set in the SceneNodes's properties, and
// not as a SceneNode that changes renderstate by itself.
//void SceneNode::SetAlpha(float alpha)
//{
//    m_Props.SetAlpha(alpha);
//    for (SceneNodeList::const_iterator i = m_Children.begin(); i != m_Children.end(); ++i)
//    {
//        shared_ptr<SceneNode> sceneNode = static_pointer_cast<SceneNode>(*i);
//        sceneNode->SetAlpha(alpha);
//    }
//}


RootNode::RootNode()
    : SceneNode(INVALID_GAMEOBJECT_ID, WeakRenderComponentPtr(), RenderPass_0, &Matrix::Identity)
{
    m_Children.reserve(RenderPass_Last);

    //shared_ptr<SceneNode> staticGroup(new SceneNode(INVALID_GAMEOBJECT_ID, WeakRenderComponentPtr(), RenderPass_Static, &Mat4x4::g_Identity));
    //m_Children.push_back(staticGroup);	// RenderPass_Static = 0

    shared_ptr<SceneNode> actorGroup(new SceneNode(INVALID_GAMEOBJECT_ID, WeakRenderComponentPtr(), RenderPass_Actor, &Matrix::Identity));
    m_Children.push_back(actorGroup);	// RenderPass_Actor = 1

    //shared_ptr<SceneNode> skyGroup(GCC_NEW SceneNode(INVALID_GAMEOBJECT_ID, WeakRenderComponentPtr(), RenderPass_Sky, &Mat4x4::g_Identity));
    //m_Children.push_back(skyGroup);	// RenderPass_Sky = 2

    //shared_ptr<SceneNode> invisibleGroup(GCC_NEW SceneNode(INVALID_GAMEOBJECT_ID, WeakRenderComponentPtr(), RenderPass_NotRendered, &Mat4x4::g_Identity));
    //m_Children.push_back(invisibleGroup);	// RenderPass_NotRendered = 3
}

//
// RootNode::VAddChild					- Chapter 16, page 546
//
bool RootNode::VAddChild(shared_ptr<ISceneNode> kid)
{
    // The Root node has children that divide the scene graph into render passes.
    // Scene nodes will get added to these children based on the value of the
    // render pass member variable.

    RenderPass pass = kid->VGet()->RenderPass();
    if ((unsigned)pass >= m_Children.size() || !m_Children[pass])
    {
        //GCC_ASSERT(0 && _T("There is no such render pass"));
        return false;
    }

    return m_Children[pass]->VAddChild(kid);
}

//
// RootNode::VRemoveChild						- not described in the book 
//
//   Returns false if nothing was removed
//
bool RootNode::VRemoveChild(GameObjectId id)
{
    bool anythingRemoved = false;
    for (int i = RenderPass_0; i<RenderPass_Last; ++i)
    {
        if (m_Children[i]->VRemoveChild(id))
        {
            anythingRemoved = true;
        }
    }
    return anythingRemoved;
}

//
// RootNode::VRenderChildren					- Chapter 16, page 547
//
HRESULT RootNode::VRenderChildren(Scene *pScene)
{
    // This code creates fine control of the render passes.

    for (int pass = RenderPass_0; pass < RenderPass_Last; ++pass)
    {
        switch (pass)
        {
        //case RenderPass_Static:
        case RenderPass_Actor:
            m_Children[pass]->VRenderChildren(pScene);
            break;

        /*case RenderPass_Sky:
        {
            shared_ptr<IRenderState> skyPass = pScene->GetRenderer()->VPrepareSkyBoxPass();
            m_Children[pass]->VRenderChildren(pScene);
            break;
        }*/
        }
    }

    return S_OK;
}


//
// D3DShaderMeshNode11::D3DShaderMeshNode11					- Chapter 16, page 562 
//
GameModelNode::GameModelNode(const GameObjectId gameObjectId,
    WeakRenderComponentPtr renderComponent,
    std::string filePath,
    RenderPass renderPass,
    const Matrix *t)
    : SceneNode(gameObjectId, renderComponent, renderPass, t)
{
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numElements = ARRAYSIZE(layout);

    m_vertexShader.Initialize(D3DRenderer11::GetInstance()->GetDevice(), L"..\\x64\\Debug\\vertexshader.cso", layout, numElements);
    
    m_pixelShader.Initialize(D3DRenderer11::GetInstance()->GetDevice(), L"..\\x64\\Debug\\pixelshader.cso");
    


    m_constantBuffer.Initialize(D3DRenderer11::GetInstance()->GetDevice(), D3DRenderer11::GetInstance()->GetDeviceContext());

    m_model.Initialize(filePath, D3DRenderer11::GetInstance()->GetDevice(), D3DRenderer11::GetInstance()->GetDeviceContext(), m_constantBuffer);
    SetPosition(Vector3::Zero);
    //VSetTransform(&m_Props.ToWorld());
    //SetRotation(Vector3::Zero);
    //UpdateMatrix();
}


//
// D3DShaderMeshNode11::VOnRestore							- Chapter 16, page 563
//
HRESULT GameModelNode::VOnRestore(Scene *pScene)
{
    //HRESULT hr;

    //hr = SceneNode::VOnRestore(pScene);

    //hr = m_VertexShader.OnRestore(pScene);
    //hr = m_PixelShader.OnRestore(pScene);

    //// Force the Mesh to reload
    //Resource resource(m_sdkMeshFileName);
    //shared_ptr<ResHandle> pResourceHandle = g_pApp->m_ResCache->GetHandle(&resource);
    //shared_ptr<D3DSdkMeshResourceExtraData11> extra = static_pointer_cast<D3DSdkMeshResourceExtraData11>(pResourceHandle->GetExtra());

    ////SetRadius(CalcBoundingSphere(&extra->m_Mesh11));

    return S_OK;
}

//
// D3DShaderMeshNode11::VRender								- Chapter 16, page 564
//
HRESULT GameModelNode::VRender(Scene *pScene)
{

    m_vertexShader.SetupRender(pScene, this);
    m_pixelShader.SetupRender(pScene, this);    
    auto scene = pScene->GetTopMatrix();

    m_model.Draw(scene, D3DRenderer11::GetInstance()->GetCamera()->GetViewMatrix() * DirectX::XMMATRIX(D3DRenderer11::GetInstance()->GetCamera()->GetProjectionMatrix()));

    return S_OK;
}

//HRESULT D3DShaderMeshNode11::VPick(Scene *pScene, RayCast *pRayCast)
//{
//    if (SceneNode::VPick(pScene, pRayCast) == E_FAIL)
//        return E_FAIL;
//
//    pScene->PushAndSetMatrix(m_Props.ToWorld());
//
//    //Get the Mesh
//    Resource resource(m_sdkMeshFileName);
//    shared_ptr<ResHandle> pResourceHandle = g_pApp->m_ResCache->GetHandle(&resource);
//    shared_ptr<D3DSdkMeshResourceExtraData11> extra = static_pointer_cast<D3DSdkMeshResourceExtraData11>(pResourceHandle->GetExtra());
//
//    HRESULT hr = pRayCast->Pick(pScene, m_Props.ActorId(), &extra->m_Mesh11);
//    pScene->PopMatrix();
//
//    return hr;
//}


//
//
//float D3DShaderMeshNode11::CalcBoundingSphere(CDXUTSDKMesh *mesh11)
//{
//    float radius = 0.0f;
//    for (UINT subset = 0; subset < mesh11->GetNumSubsets(0); ++subset)
//    {
//        Vec3 extents = mesh11->GetMeshBBoxExtents(subset);
//        extents.x = abs(extents.x);
//        extents.y = abs(extents.y);
//        extents.z = abs(extents.z);
//        radius = (radius > extents.x) ? radius : extents.x;
//        radius = (radius > extents.y) ? radius : extents.y;
//        radius = (radius > extents.z) ? radius : extents.z;
//    }
//    return radius;
//}
