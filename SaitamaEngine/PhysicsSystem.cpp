#include "pch.h"
#include "PhysicsSystem.h"
#include "Particles.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Events.h"
#include "D3DRenderer11.h"
#include "PhysicsDef.h"
#include "Transform.h"



using namespace PhysicsDef;

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
    //Broad Phase Collision Detection
    m_colliderPairList.clear();
    for (int i = 0; i < m_rigidBodies.size(); i++)
    {
        for (int j = i + 1; j < m_rigidBodies.size(); j++)
        {
            m_colliderPairList.push_back(CollisionPair(m_rigidBodies[i], m_rigidBodies[j]));
        }
    }

    // Integrate forces
    for (auto r : m_rigidBodies)
    {
        //r->integrate(delta);
    }
    
    std::vector<Manifold> newManifolds;

    for (int i = 0; i < m_colliderPairList.size();i++)
    {
        Manifold manifold(m_colliderPairList[i].first, m_colliderPairList[i].second);
        if (gjk.CollisionDetection(m_colliderPairList[i].first, m_colliderPairList[i].second, manifold))
        {
            newManifolds.push_back(manifold);
            //m_persistentManifold.addContactManifold(&manifold);

            ManifoldMap manifoldMap;
            ManifoldMapIter it;

            for (int i = 0; i < newManifolds.size(); ++i)
            {
                ManifoldKey key(newManifolds[i].m_bodyA, newManifolds[i].m_bodyB);

                it = m_manifolds.find(key);

                //If no manifold is found, add it and move on
                if (it == m_manifolds.end())
                {
                    manifoldMap.emplace(key, newManifolds[i]);

                    continue;
                }

                //Otherwise we need to merge
                it->second.update(newManifolds[i].m_contacts.data(), newManifolds[i].m_contacts.size());// , newManifolds[i].m_contactCount);
                it->second.m_isPersistent = true;
                //Add it to the new map, this step needs to be improved
                manifoldMap.emplace(it->first, it->second);
                newManifolds[i].m_isPersistent = true;
                newManifolds[i].m_contacts = it->second.m_contacts;
            }
            m_manifolds = manifoldMap;
            m_constraintSolver.SolveConstraints2(newManifolds, delta);

        } else
        {
            m_manifolds.clear();
        }

        
    }


    //Perform Movement

    float angMag;
    Vector3 linVel, angVel, pos;
    Transform trans, finalTrans;
    Quaternion rot;
    for (unsigned int i = 0; i < m_rigidBodies.size(); ++i)
    {
        linVel = m_rigidBodies[i]->getLinearVelocity();
        angVel = m_rigidBodies[i]->getAngularVelocity();
        angMag = angVel.Length();


        trans = m_rigidBodies[i]->getTransform();

        rot = trans.getOrientation();
        pos = trans.getPosition();

        rot *= Quaternion(Vector4(angVel.x, angVel.y, angVel.z, 1.0f) * delta);
        pos += linVel * delta;
        rot.Normalize(rot);
        trans.setPosition(pos);
        trans.setOrientation(rot);
        if (linVel.Dot(linVel) < 0.00001)
        {
            linVel = Vector3::Zero;
            m_rigidBodies[i]->setLinearVelocity(linVel);
        }
        if (angVel.Dot(angVel) < 0.00001)
        {
            angVel = Vector3::Zero;
            m_rigidBodies[i]->setAngularVelocity(angVel);
        }

        m_rigidBodies[i]->updateTransform(trans);
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

    RigidBodyConstructionInfo rbci;
    
    std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr<TransformComponent>(pGameObject->GetComponent<TransformComponent>(TransformComponent::g_Name));
    rbci.transform.setPosition(pTransformComponent->GetPosition());
    //rbci.collisionShape = new CollisionBox(Vector3(1.0f, 1.0f, 1.0f));
    rbci.collisionShape = new CollisionBox(Vector3(1.0f, 1.0f, 1.0f));
    rbci.mass = 2.0f;
    rbci.linearDamping = 0.f;
    rbci.angularDamping = 0.f;
    rbci.friction = 0.3f;
    rbci.rollingFriction = 0.3f;
    rbci.resititution = 0.5f;
    rbci.localInertia = Vector3(0, 0, 0); //TODO: INCORRECT, add calculator funciton;

    RigidBody *r = new RigidBody(rbci);
    //r->setLinearVelocity(Vector3(0.0f, 0.0f, 1.0f));
    
    ///r->setAcceleration(0.0f, 0.0f, 0.0f);
    //r->setDamping(0.99f, 0.99f);

    //std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr<TransformComponent>(pGameObject->GetComponent<TransformComponent>(TransformComponent::g_Name));
    if (pTransformComponent->GetPosition().z > 0)
    {
        r->setLinearVelocity(Vector3(0.0f, 0.0f, -1.0f));
        //r->setVelocity(0.0f, 0.0f, -2.0f);
    } else
    {
        r->setLinearVelocity(Vector3(0.0f, 0.0f, 1.0f));
        //r->setVelocity(0.0f, 0.0f, 2.0f);
    }
    //r->setPosition(pTransformComponent->GetPosition());
    

    /*CollisionBox *cb = new CollisionBox();
    cb->pos = r->getPosition();
    cb->min = Vector3(0, 0, 0);
    cb->max = Vector3(1, 1, 1);

    r->AddCollider(cb);*/

    m_rigidBodies.push_back(r);

    m_gameObjectIdToRigidBody[gameObjectId] = r;
}

void PhysicsSystem::VAddCollider(StrongGameObjectPtr pGameObject)
{

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
                if (pTransformComponent->GetPosition() != it->second->getTransform().getPosition())
                {                    
                    Matrix m = pTransformComponent->GetTransform();
                    m.Translation(it->second->getTransform().getPosition());
                    pTransformComponent->SetTransform(m);
                    shared_ptr<EvtData_Move_GameObject> pEvent(new EvtData_Move_GameObject(id, m));
                    IEventManager::Get()->VQueueEvent(pEvent);
                }
            }
        }
    }
}