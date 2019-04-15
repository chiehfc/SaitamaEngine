#include "pch.h"
#include "EventManager.h"


static IEventManager* g_pEventMgr = NULL;
GenericObjectFactory<IEventData, EventType> g_eventFactory;

//GCC_MEMORY_WATCHER_DEFINITION(IEventData);

IEventManager* IEventManager::Get(void)
{
    return g_pEventMgr;
}

IEventManager::IEventManager(const char* pName, bool setAsGlobal)
{
    if (setAsGlobal)
    {
        if (g_pEventMgr)
        {
            delete g_pEventMgr;
        }

        g_pEventMgr = this;
    }
}

IEventManager::~IEventManager(void)
{
    if (g_pEventMgr == this)
        g_pEventMgr = NULL;
}

