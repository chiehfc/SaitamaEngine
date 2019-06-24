#include "pch.h"
#include "PhysicsComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Game.h"

const char *PhysicsComponent::g_Name = "PhysicsComponent";

PhysicsComponent::PhysicsComponent()
{
}


PhysicsComponent::~PhysicsComponent()
{
}

bool PhysicsComponent::VInit(tinyxml2::XMLElement * pData)
{
    m_pGamePhysics = Game::GetInstance()->GetPhysics();
    if (!m_pGamePhysics)
        return false;

    
    return true;
}

void PhysicsComponent::VPostInit(void)
{

    m_pGamePhysics->VAddRigidBody(m_pOwner);

    // Create actual rigidbody for physics system to use
    if (m_pOwner)
    {        
        /*if (m_shape == "Sphere")
        {
            m_pGamePhysics->VAddSphere((float)m_RigidBodyScale.x, m_pOwner, m_density, m_material);
        } else if (m_shape == "Box")
        {
            m_pGamePhysics->VAddBox(m_RigidBodyScale, m_pOwner, m_density, m_material);
        } else if (m_shape == "PointCloud")
        {
            GCC_ERROR("Not supported yet!");
        }*/
    }
}

void PhysicsComponent::VUpdate(int deltaMs)
{
    m_pGamePhysics->OnUpdate(deltaMs);
}

void PhysicsComponent::ApplyForce(const Vector3 &direction, float forceNewtons)
{
}

void PhysicsComponent::ApplyAcceleration(float acceleration)
{
    m_acceleration = acceleration;
}

Vector3 PhysicsComponent::GetVelocity(void)
{
    return m_pGamePhysics->VGetVelocity(m_pOwner->GetId());
}

void PhysicsComponent::SetVelocity(const Vector3 &velocity)
{
    m_pGamePhysics->VSetVelocity(m_pOwner->GetId(), velocity);
}

void PhysicsComponent::SetPosition(float x, float y, float z)
{
    shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
    if (pTransformComponent)
    {
        Matrix transform = pTransformComponent->GetTransform();
        Vector3 position = Vector3(x, y, z);
        transform.Translation(position);

        //KinematicMove(transform);
    }
}

void PhysicsComponent::Stop(void)
{    
    //return m_pGamePhysics->VStopGameObject(m_pOwner->GetId());
    return;
}
