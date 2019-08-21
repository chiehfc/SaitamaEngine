#pragma once
#include "Saitama.h"
#include "CollisionPrimitive.h"
#include "Transform.h"

class RigidBody
{
public:    
    RigidBody(const PhysicsDef::RigidBodyConstructionInfo& rbci);
    ~RigidBody() {};

protected:
    
    float mass;

    float inverseMass;

    Matrix inverseInertiaTensor;
        
    double linearDamping;

    double angularDamping;
        
    Vector3 position;

    Quaternion orientation;

    Vector3 linearVelocity;

    Vector3 angularVelocity;

    Matrix inverseInertiaTensorWorld;

    Transform transformMatrix;

    Vector3 forceAccum;

    Vector3 torqueAccum;

    Vector3 acceleration;

    float friction;
    //float rollingFriction;
    float restitution;

public:

    /**
     * Integrates the rigid body forward in time by the given amount.
     * This function uses a Newton-Euler integration method, which is a
     * linear approximation to the correct integral. For this reason it
     * may be inaccurate in some cases.
     */
    void integrate(double duration);

    void setMass(const double mass);
    double getMass() const;

    void setInverseMass(const double inverseMass);
    double getInverseMass() const;

    bool hasFiniteMass() const;

    void setInertiaTensor(const Matrix &inertiaTensor);    

    Matrix getInertiaTensor() const;

    Matrix getInertiaTensorWorld() const;

    void setInverseInertiaTensor(const Matrix &inverseInertiaTensor);

    Matrix getInverseInertiaTensor() const;

    Matrix getInverseInertiaTensorWorld() const;

    float getFriction() const;

    float getRestitution() const;

    void setDamping(const double linearDamping, const double angularDamping);
    void setLinearDamping(const double linearDamping);
    double getLinearDamping() const;
    void setAngularDamping(const double angularDamping);
    double getAngularDamping() const;
    
    Transform getTransform() const;

    Vector3 getPointInLocalSpace(const Vector3 &worldPoint) const;
    Vector3 getPointInWorldSpace(const Vector3 &localPoint) const;
    Vector3 getDirectionInLocalSpace(const Vector3 &worldDirection) const;
    Vector3 getDirectionInWorldSpace(const Vector3 &localDirection) const;

    void setLinearVelocity(const Vector3 &linearVelocity);

    Vector3 getLinearVelocity() const;

    void addLinearVelocity(const Vector3 &deltaLinearVelocity);

    void setAngularVelocity(const Vector3 &angularVelocity);

    Vector3 getAngularVelocity() const;

    void addAngularVelocity(const Vector3 &deltaAngularVelocity);

    void clearAccumulators();

    void addForce(const Vector3 &force);

    void addTorque(const Vector3 &torque);

    void setAcceleration(const Vector3 &acceleration);

    Vector3 getAcceleration() const;

    /*@}*/
    // return local position
    Vector3 getSupportPoint(Vector3 direction) const;

    CollisionPrimitive *collider;


    void applyImpulse(const Vector3 &impulse);
    void applyTorqueImpulse(const Vector3 &torque);
    void updateTransform(Transform &transform) { transformMatrix = transform; }
};