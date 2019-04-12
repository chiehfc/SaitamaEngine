#include "pch.h"
#include "Saitama.h"
#include "GameObjectFactory.h"
//#include "../ResourceCache/XmlResource.h"
#include "GameObjectComponent.h"
//#include "AudioComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
//#include "PhysicsComponent.h"
//#include "AmmoPickup.h"
//#include "HealthPickup.h"
//#include "BaseScriptComponent.h"
#include "GameObject.h"
#include "StringHelper.h"
#include "XMLLoader.h"
#include <iostream>

//---------------------------------------------------------------------------------------------------------------------
// Factory class definition
// Chapter 6, page 161
//---------------------------------------------------------------------------------------------------------------------
GameObjectFactory::GameObjectFactory(void)
{
    m_lastGameObjectId = INVALID_GAMEOBJECT_ID;
    
    m_componentFactory.Register<TransformComponent>(GameObjectComponent::GetIdFromName(TransformComponent::g_Name));
    m_componentFactory.Register<RenderComponent>(GameObjectComponent::GetIdFromName(RenderComponent::g_Name));
    m_componentFactory.Register<LightRenderComponent>(GameObjectComponent::GetIdFromName(LightRenderComponent::g_Name));
    /*
    m_componentFactory.Register<MeshRenderComponent>(GameObjectComponent::GetIdFromName(MeshRenderComponent::g_Name));
    m_componentFactory.Register<SphereRenderComponent>(GameObjectComponent::GetIdFromName(SphereRenderComponent::g_Name));
    m_componentFactory.Register<PhysicsComponent>(GameObjectComponent::GetIdFromName(PhysicsComponent::g_Name));
    
    m_componentFactory.Register<GridRenderComponent>(GameObjectComponent::GetIdFromName(GridRenderComponent::g_Name));
    
    m_componentFactory.Register<SkyRenderComponent>(GameObjectComponent::GetIdFromName(SkyRenderComponent::g_Name));
    m_componentFactory.Register<AudioComponent>(GameObjectComponent::GetIdFromName(AudioComponent::g_Name));

    // FUTURE WORK - certainly don't need to do this now, but the following stuff should be in a TeapotWarsActorFactory, eh?
    m_componentFactory.Register<AmmoPickup>(GameObjectComponent::GetIdFromName(AmmoPickup::g_Name));
    m_componentFactory.Register<HealthPickup>(GameObjectComponent::GetIdFromName(HealthPickup::g_Name));
    m_componentFactory.Register<BaseScriptComponent>(GameObjectComponent::GetIdFromName(BaseScriptComponent::g_Name));

    */
}

StrongGameObjectPtr GameObjectFactory::CreateGameObject(const char* gameObjectResource, tinyxml2::XMLElement *overrides, const DirectX::XMMATRIX *pInitialTransform, const GameObjectId serversGameObjectId)
{
    XMLLoader xml;
    tinyxml2::XMLNode *pRoot = xml.GetRoot();

    for (tinyxml2::XMLElement* pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
    {
        std::cout << pNode->Value() << std::endl;
    }

    

    //tinyxml2::XMLError eResult = xml_doc.LoadFile("test.xml");
    //if (eResult != tinyxml2::XML_SUCCESS) return false;

    //tinyxml2::XMLNode* root = xml_doc.FirstChildElement("empleados");

    //// Grab the root XML node
    //tinyxml2::XMLElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement(actorResource);
    //if (!pRoot)
    //{
    //    GCC_ERROR("Failed to create actor from resource: " + std::string(actorResource));
    //    return StrongActorPtr();
    //}

    //// create the actor instance
    GameObjectId nextGameObjectId = serversGameObjectId;
    if (nextGameObjectId == INVALID_GAMEOBJECT_ID)
    {
        nextGameObjectId = GetNextActorId();
    }
    StrongGameObjectPtr pGameObject(new GameObject(nextGameObjectId));
    if (!pGameObject->Init(pRoot->ToElement()))
    {
        //GCC_ERROR("Failed to initialize actor: " + std::string(gameObjectResource));
        return StrongGameObjectPtr();
    }

    //bool initialTransformSet = false;

    //// Loop through each child element and load the component
    for (tinyxml2::XMLElement* pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
    {
        StrongGameObjectComponentPtr pComponent(VCreateComponent(pNode));
        if (pComponent)
        {
            pGameObject->AddComponent(pComponent);
            pComponent->SetOwner(pGameObject);
        } else
        {
            // If an error occurs, we kill the actor and bail.  We could keep going, but the actor is will only be 
            // partially complete so it's not worth it.  Note that the pActor instance will be destroyed because it
            // will fall out of scope with nothing else pointing to it.
            return StrongGameObjectPtr();
        }
    }

    //if (overrides)
    //{
    //    ModifyActor(pActor, overrides);
    //}

    //// This is a bit of a hack to get the initial transform of the transform component set before the 
    //// other components (like PhysicsComponent) read it.
    //std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr<TransformComponent>(pGameObject->GetComponent<TransformComponent>(TransformComponent::g_Name));
    //if (pInitialTransform && pTransformComponent)
    //{
    //    pTransformComponent->SetPosition(pInitialTransform->GetPosition());
    //}

    //// Now that the actor has been fully created, run the post init phase
    //pActor->PostInit();

    return pGameObject;
}

StrongGameObjectComponentPtr GameObjectFactory::VCreateComponent(tinyxml2::XMLElement* pData)
{
    const char* name = pData->Value();
    StrongGameObjectComponentPtr pComponent(m_componentFactory.Create(GameObjectComponent::GetIdFromName(name)));

    // initialize the component if we found one
    if (pComponent)
    {
        if (!pComponent->VInit(pData))
        {
            //GCC_ERROR("Component failed to initialize: " + std::string(name));
            return StrongGameObjectComponentPtr();
        }
    } else
    {
        //GCC_ERROR("Couldn't find GameObjectComponent named " + std::string(name));
        return StrongGameObjectComponentPtr();  // fail
    }

    // pComponent will be NULL if the component wasn't found.  This isn't necessarily an error since you might have a 
    // custom CreateComponent() function in a sub class.
    return pComponent;
}


void GameObjectFactory::ModifyGameObject(StrongGameObjectPtr pGameObject, tinyxml2::XMLElement *overrides)
{
    //// Loop through each child element and load the component
    //for (tinyxml2::XMLElement *pNode = overrides->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
    //{
    //    ComponentId componentId = GameObjectComponent::GetIdFromName(pNode->Value());
    //    StrongGameObjectComponentPtr pComponent = MakeStrongPtr(pActor->GetComponent<GameObjectComponent>(componentId));
    //    if (pComponent)
    //    {
    //        pComponent->VInit(pNode);

    //        // [mrmike] - added post press to ensure that components that need it have
    //        //            Events generated that can notify subsystems when changes happen.
    //        //            This was done to have SceneNode derived classes respond to RenderComponent
    //        //            changes.

    //        pComponent->VOnChanged();
    //    } else
    //    {
    //        pComponent = VCreateComponent(pNode);
    //        if (pComponent)
    //        {
    //            pActor->AddComponent(pComponent);
    //            pComponent->SetOwner(pActor);
    //        }
    //    }
    //}
}