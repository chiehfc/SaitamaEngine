#pragma once

#include "Saitama.h"
#include "GameObjectComponent.h"
#include "PhysicsSystem.h"

class PhysicsComponent : public GameObjectComponent
{
public:
    PhysicsComponent(void);
    virtual ~PhysicsComponent(void);

    static const char *g_Name;
    virtual const char *VGetName() const override { return PhysicsComponent::g_Name; }

    virtual bool VInit(tinyxml2::XMLElement *pData) override;
    virtual void VPostInit(void) override;
    virtual void VUpdate(int deltaMs) override;


    void ApplyForce(const Vector3& direction, float forceNewtons);
    void ApplyAcceleration(float acceleration);

    Vector3 GetVelocity(void);
    void SetVelocity(const Vector3& velocity);
    
    void SetPosition(float x, float y, float z);
    void Stop(void);

    double m_acceleration;
    double m_velocity;

    shared_ptr<PhysicsSystem> m_pGamePhysics;  // might be better as a weak ptr :/
};

