#include "pch.h"
#include "PhysicsSystem.h"
#include "Particles.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Events.h"
#include "D3DRenderer11.h"

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
    for (auto r : m_rigidBodies) 
    {
        r->integrate(delta);
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
    p->setVelocity(0.0f, 0.0f, 2.0f); // 35m/s
    p->setAcceleration(0.0f, 0.0f, 0.0f);
    p->setDamping(0.99f);

    m_particles.push_back(p);

    m_gameObjectIdToParticle[gameObjectId] = p;
}

void PhysicsSystem::VAddRigidBody(StrongGameObjectPtr pGameObject)
{
    GameObjectId gameObjectId = pGameObject->GetId();

    RigidBody *r = new RigidBody();
    r->setMass(2.0f); // 2.0kg
    r->setVelocity(0.0f, 0.0f, 2.0f); // 35m/s
    r->setAcceleration(0.0f, 0.0f, 0.0f);
    r->setDamping(0.99f, 0.99f);

    m_rigidBodies.push_back(r);

    m_gameObjectIdToRigidBody[gameObjectId] = r;
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

RigidBody *PhysicsSystem::FindRigidBody(GameObjectId id) const
{
    GameObjectIDToRigidBodyMap::const_iterator it = m_gameObjectIdToRigidBody.find(id);
    if (it != m_gameObjectIdToRigidBody.end())
    {
        return it->second;
    }
    return nullptr;
}

void PhysicsSystem::VSyncVisibleScene()
{
    // Keep physics & graphics in sync

    // check all the existing actor's bodies for changes. 
    //  If there is a change, send the appropriate event for the game system.
    for (GameObjectIDToRigidBodyMap::const_iterator it = m_gameObjectIdToRigidBody.begin();
        it != m_gameObjectIdToRigidBody.end();
        ++it)
    {
        const GameObjectId id = it->first;
        
        StrongGameObjectPtr pGameActor = MakeStrongPtr(D3DRenderer11::GetInstance()->VGetGameObject(id));
        if (pGameActor)
        {
            shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(pGameActor->GetComponent<TransformComponent>(TransformComponent::g_Name));
            if (pTransformComponent)
            {
                if (pTransformComponent->GetPosition() != it->second->getPosition())
                {                    
                    Matrix m = pTransformComponent->GetTransform();
                    m.Translation(it->second->getPosition());
                    pTransformComponent->SetTransform(m);
                    shared_ptr<EvtData_Move_GameObject> pEvent(new EvtData_Move_GameObject(id, m));
                    IEventManager::Get()->VQueueEvent(pEvent);
                }
            }
        }
    }
}