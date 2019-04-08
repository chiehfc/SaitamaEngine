#include "pch.h"
#include "Scene.h"


Scene::Scene(shared_ptr<IRenderer> renderer)
{
    m_Root.reset(new RootNode());
    m_Renderer = renderer;
    //m_LightManager = GCC_NEW LightManager;

    m_MatrixStack = new MatrixStack();

    // [mrmike] - event delegates were added post-press
    /*IEventManager* pEventMgr = IEventManager::Get();
    pEventMgr->VAddListener(MakeDelegate(this, &Scene::NewRenderComponentDelegate), EvtData_New_Render_Component::sk_EventType);
    pEventMgr->VAddListener(MakeDelegate(this, &Scene::DestroyActorDelegate), EvtData_Destroy_Actor::sk_EventType);
    pEventMgr->VAddListener(MakeDelegate(this, &Scene::MoveActorDelegate), EvtData_Move_Actor::sk_EventType);
    pEventMgr->VAddListener(MakeDelegate(this, &Scene::ModifiedRenderComponentDelegate), EvtData_Modified_Render_Component::sk_EventType);*/
}

//
// Scene::~Scene					- Chapter 16, page 539
//
Scene::~Scene()
{
    // [mrmike] - event delegates were added post-press!
    /*IEventManager* pEventMgr = IEventManager::Get();
    pEventMgr->VRemoveListener(MakeDelegate(this, &Scene::NewRenderComponentDelegate), EvtData_New_Render_Component::sk_EventType);
    pEventMgr->VRemoveListener(MakeDelegate(this, &Scene::DestroyActorDelegate), EvtData_Destroy_Actor::sk_EventType);
    pEventMgr->VRemoveListener(MakeDelegate(this, &Scene::MoveActorDelegate), EvtData_Move_Actor::sk_EventType);

    pEventMgr->VRemoveListener(MakeDelegate(this, &Scene::ModifiedRenderComponentDelegate), EvtData_Modified_Render_Component::sk_EventType);*/

    delete m_MatrixStack;
    //SAFE_DELETE(m_LightManager);
}

//
// Scene::OnRender					- Chapter 16, page 539
//
HRESULT Scene::OnRender()
{
    // The render passes usually go like this 
    // 1. Static objects & terrain
    // 2. Actors (dynamic objects that can move)
    // 3. The Sky
    // 4. Anything with Alpha

    if (m_Root /*&& m_Camera*/)
    {
        // The scene root could be anything, but it
        // is usually a SceneNode with the identity
        // matrix

        //m_Camera->SetViewTransform(this);

        //m_LightManager->CalcLighting(this);

        if (m_Root->VPreRender(this) == S_OK)
        {
            m_Root->VRender(this);
            m_Root->VRenderChildren(this);
            m_Root->VPostRender(this);
        }
        //RenderAlphaPass();
    }

    return S_OK;
}

//
// Scene::OnLostDevice						- not in the book
//
//    All Scene nodes implement VOnLostDevice, which is called in the D3D9 renderer.
//
HRESULT Scene::OnLostDevice()
{
    if (m_Root)
    {
        return m_Root->VOnLostDevice(this);
    }
    return S_OK;
}

//
// Scene::OnRestore					- Chapter 16, page 540
//
HRESULT Scene::OnRestore()
{
    if (!m_Root)
        return S_OK;

    HRESULT hr;
    hr = m_Renderer->VOnRestore();

    return m_Root->VOnRestore(this);
}





bool Scene::AddChild(GameObjectId id, shared_ptr<ISceneNode> kid)
{
    if (id != INVALID_GAMEOBJECT_ID)
    {
        // This allows us to search for this later based on actor id
        m_GameObjectMap[id] = kid;
    }

    /*shared_ptr<LightNode> pLight = dynamic_pointer_cast<LightNode>(kid);
    if (pLight != NULL && m_LightManager->m_Lights.size() + 1 < MAXIMUM_LIGHTS_SUPPORTED)
    {
        m_LightManager->m_Lights.push_back(pLight);
    }*/
    return m_Root->VAddChild(kid);
}

bool Scene::RemoveChild(GameObjectId id)
{
    if (id == INVALID_GAMEOBJECT_ID)
        return false;
    shared_ptr<ISceneNode> kid = FindGameObject(id);
    //shared_ptr<LightNode> pLight = dynamic_pointer_cast<LightNode>(kid);
    //if (pLight != NULL)
    //{
    //    m_LightManager->m_Lights.remove(pLight);
    //}
    m_GameObjectMap.erase(id);
    return m_Root->VRemoveChild(id);
}


//
//void Scene::NewRenderComponentDelegate(IEventDataPtr pEventData)
//{
//    shared_ptr<EvtData_New_Render_Component> pCastEventData = static_pointer_cast<EvtData_New_Render_Component>(pEventData);
//
//    ActorId actorId = pCastEventData->GetActorId();
//    shared_ptr<SceneNode> pSceneNode(pCastEventData->GetSceneNode());
//
//    // FUTURE WORK: Add better error handling here.		
//    if (FAILED(pSceneNode->VOnRestore(this)))
//    {
//        std::string error = "Failed to restore scene node to the scene for actorid " + ToStr(actorId);
//        GCC_ERROR(error);
//        return;
//    }
//
//    AddChild(actorId, pSceneNode);
//}
//
//void Scene::ModifiedRenderComponentDelegate(IEventDataPtr pEventData)
//{
//    shared_ptr<EvtData_Modified_Render_Component> pCastEventData = static_pointer_cast<EvtData_Modified_Render_Component>(pEventData);
//
//    ActorId actorId = pCastEventData->GetActorId();
//    if (actorId == INVALID_ACTOR_ID)
//    {
//        GCC_ERROR("Scene::ModifiedRenderComponentDelegate - unknown actor id!");
//        return;
//    }
//
//    if (g_pApp->GetGameLogic()->GetState() == BGS_LoadingGameEnvironment)
//        return;
//
//    shared_ptr<ISceneNode> pSceneNode = FindActor(actorId);
//    // FUTURE WORK: Add better error handling here.		
//    if (!pSceneNode || FAILED(pSceneNode->VOnRestore(this)))
//    {
//        GCC_ERROR("Failed to restore scene node to the scene for actorid " + ToStr(actorId));
//    }
//}
//
//void Scene::DestroyActorDelegate(IEventDataPtr pEventData)
//{
//    shared_ptr<EvtData_Destroy_Actor> pCastEventData = static_pointer_cast<EvtData_Destroy_Actor>(pEventData);
//    RemoveChild(pCastEventData->GetId());
//}
//
//void Scene::MoveActorDelegate(IEventDataPtr pEventData)
//{
//    shared_ptr<EvtData_Move_Actor> pCastEventData = static_pointer_cast<EvtData_Move_Actor>(pEventData);
//
//    ActorId id = pCastEventData->GetId();
//    Mat4x4 transform = pCastEventData->GetMatrix();
//
//    shared_ptr<ISceneNode> pNode = FindActor(id);
//    if (pNode)
//    {
//        pNode->VSetTransform(&transform);
//    }
//}



//
// Scene::OnUpdate					- Chapter 16, page 540
//
HRESULT Scene::OnUpdate(const int deltaMilliseconds)
{
    if (!m_Root)
        return S_OK;

    /*static DWORD lastTime = timeGetTime();
    DWORD elapsedTime = 0;
    DWORD now = timeGetTime();

    elapsedTime = now - lastTime;
    lastTime = now;*/

    return m_Root->VOnUpdate(this, deltaMilliseconds);
}

//
// Scene::FindActor					- Chapter 16, page 542
//
shared_ptr<ISceneNode> Scene::FindGameObject(GameObjectId id)
{
    SceneGameObjectMap::iterator i = m_GameObjectMap.find(id);
    if (i == m_GameObjectMap.end())
    {
        return shared_ptr<ISceneNode>();
    }

    return i->second;
}


//
// Scene::RenderAlphaPass			- Chapter 16, page 543
//
//void Scene::RenderAlphaPass()
//{
//    shared_ptr<IRenderState> alphaPass = m_Renderer->VPrepareAlphaPass();
//
//    m_AlphaSceneNodes.sort();
//    while (!m_AlphaSceneNodes.empty())
//    {
//        AlphaSceneNodes::reverse_iterator i = m_AlphaSceneNodes.rbegin();
//        PushAndSetMatrix((*i)->m_Concat);
//        (*i)->m_pNode->VRender(this);
//        delete (*i);
//        PopMatrix();
//        m_AlphaSceneNodes.pop_back();
//    }
//}



