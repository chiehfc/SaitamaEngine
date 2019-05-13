#include "pch.h"
#include "PhysicsComponent.h"


PhysicsComponent::PhysicsComponent()
{
}


PhysicsComponent::~PhysicsComponent()
{
}

bool PhysicsComponent::VInit(tinyxml2::XMLElement * pData)
{
    return false;
}

void PhysicsComponent::VPostInit(void)
{
}

void PhysicsComponent::VUpdate(int deltaMs)
{
    m_pGamePhysics->OnUpdate(deltaMs);
}

void PhysicsComponent::ApplyForce(const Vector3 & direction, float forceNewtons)
{
}

void PhysicsComponent::ApplyAcceleration(float acceleration)
{
}

Vector3 PhysicsComponent::GetVelocity(void)
{
    return Vector3();
}

void PhysicsComponent::SetVelocity(const Vector3 & velocity)
{
}

void PhysicsComponent::SetPosition(float x, float y, float z)
{
}

void PhysicsComponent::Stop(void)
{
}
