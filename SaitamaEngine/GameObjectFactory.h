#pragma once
#include <memory>
#include "GameObject.h"
#include "Saitama.h"



class GameObjectFactory
{
    GameObjectId m_lastActorId;

protected:
    GenericObjectFactory<GameObjectComponent, ComponentId> m_componentFactory;

public:
    GameObjectFactory(void);

    StrongActorPtr CreateActor(const char* actorResource, tinyxml2::XMLElement* overrides, const DirectX::XMMATRIX* initialTransform, const GameObjectId serversActorId);
    void ModifyActor(StrongActorPtr pActor, tinyxml2::XMLElement* overrides);

    //protected:
        // [rez] This function can be overridden by a subclass so you can create game-specific C++ components.  If you do
        // this, make sure you call the base-class version first.  If it returns NULL, you know it's not an engine component.
    virtual StrongGameObjectComponentPtr VCreateComponent(tinyxml2::XMLElement* pData);

private:
    GameObjectId GetNextActorId(void) { ++m_lastActorId; return m_lastActorId; }
};

