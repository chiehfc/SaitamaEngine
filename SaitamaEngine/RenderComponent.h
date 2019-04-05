#pragma once
#include "Saitama.h"
#include "GameObjectComponent.h"

class RenderComponent : public GameObjectComponent
{
public:
    virtual bool VInit(tinyxml2::XMLElement *pData) override;
    virtual void VPostInit() override;
    virtual void VOnChanged() override;
    
};

