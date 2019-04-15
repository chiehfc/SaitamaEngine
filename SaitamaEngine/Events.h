#pragma once
#include "EventManager.h"
#include "SceneNode.h"

class EvtData_New_Render_Component : public BaseEventData
{
    GameObjectId m_gameObjectId;
    shared_ptr<SceneNode> m_pSceneNode;

public:
    static const EventType sk_EventType;

    EvtData_New_Render_Component(void)
    {
        m_gameObjectId = INVALID_GAMEOBJECT_ID;
    }

    explicit EvtData_New_Render_Component(GameObjectId gameObjectId, shared_ptr<SceneNode> pSceneNode)
        : m_gameObjectId(gameObjectId),
        m_pSceneNode(pSceneNode)
    {
    }

    /*virtual void VSerialize(std::ostrstream& out) const
    {
        GCC_ERROR(GetName() + std::string(" should not be serialzied!"));
    }

    virtual void VDeserialize(std::istrstream& in)
    {
        GCC_ERROR(GetName() + std::string(" should not be serialzied!"));
    }*/

    virtual const EventType& VGetEventType(void) const
    {
        return sk_EventType;
    }

    virtual IEventDataPtr VCopy(void) const
    {
        return IEventDataPtr(new EvtData_New_Render_Component(m_gameObjectId, m_pSceneNode));
    }

    virtual const char* GetName(void) const
    {
        return "EvtData_New_Render_Component";
    }

    const GameObjectId GetGameObjectId(void) const
    {
        return m_gameObjectId;
    }

    shared_ptr<SceneNode> GetSceneNode(void) const
    {
        return m_pSceneNode;
    }
};
