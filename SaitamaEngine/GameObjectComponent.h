#pragma once
#include "Saitama.h"
#include "StringHelper.h"

class GameObjectComponent
{
    friend class ActorFactory;

protected:
    StrongActorPtr m_pOwner;

public:
    virtual ~GameObjectComponent(void) { m_pOwner.reset(); }

    // These functions are meant to be overridden by the implementation classes of the components.
    virtual bool VInit(tinyxml2::XMLElement* pData) = 0;
    virtual void VPostInit(void) { }
    virtual void VUpdate(int deltaMs) { }
    virtual void VOnChanged(void) { }				

    // for the editor
    virtual tinyxml2::XMLElement* VGenerateXml(void) = 0;

    // This function should be overridden by the interface class.
    virtual ComponentId VGetId(void) const { return GetIdFromName(VGetName()); }
    virtual const char *VGetName() const = 0;
    static ComponentId GetIdFromName(const char* componentStr)
    {
        void* rawId = HashedString::hash_name(componentStr);
        return reinterpret_cast<ComponentId>(rawId);
    }

private:
    void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }

};


