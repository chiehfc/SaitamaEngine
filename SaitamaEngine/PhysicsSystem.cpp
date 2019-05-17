#include "pch.h"
#include "PhysicsSystem.h"
#include "Particles.h"
#include "GameObject.h"


PhysicsSystem::PhysicsSystem()
{
}


PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::Initialize()
{

}

void PhysicsSystem::OnUpdate(double delta)
{
    for (auto p : m_particles) 
    {
        p->integrate(delta);
    }    
}

void PhysicsSystem::VApplyForce(const Vector3 &dir, float newtons, GameObjectId gameObjectId)
{
}

void PhysicsSystem::VAddParticle(StrongGameObjectPtr pGameObject)
{
    GameObjectId gameObjectId = pGameObject->GetId();
        
    Particles *p = new Particles();
    p->setMass(2.0f); // 2.0kg
    p->setVelocity(0.0f, 0.0f, 35.0f); // 35m/s
    p->setAcceleration(0.0f, -1.0f, 0.0f);
    p->setDamping(0.99f);

    m_particles.push_back(p);

    m_gameObjectIdToParticle[gameObjectId] = p;
}

void PhysicsSystem::VRemoveGameObject(GameObjectId id)
{
}

Vector3 PhysicsSystem::VGetVelocity(GameObjectId gameObjectId)
{
    return Vector3();
}

void PhysicsSystem::VSetVelocity(GameObjectId gameObjectId, const Vector3 &vel)
{
    Particles *particle = FindParticle(gameObjectId);

    if (!particle) 
    {
        return;
    }
    particle->setVelocity(vel);
}

void PhysicsSystem::VTranslate(GameObjectId gameObjectId, const Vector3 &vec)
{

}

void PhysicsSystem::VSetTransform(const GameObjectId id, const Vector3 &mat)
{

}

Matrix PhysicsSystem::VGetTransform(const GameObjectId id)
{
    return Matrix();
}


Particles *PhysicsSystem::FindParticle(GameObjectId id) const
{
    GameObjectIDToParticleMap::const_iterator it = m_gameObjectIdToParticle.find(id);
    if (it != m_gameObjectIdToParticle.end()) 
    {
        return it->second;
    }
    return nullptr;
}