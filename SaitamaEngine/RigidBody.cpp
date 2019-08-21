#include "pch.h"
#include "RigidBody.h"

RigidBody::RigidBody(const PhysicsDef::RigidBodyConstructionInfo& rbci)
{
    mass = rbci.mass;

    if (mass == 0)
        inverseMass = 0;
    else
        inverseMass = 1 / mass;
    linearDamping = rbci.linearDamping;
    angularDamping = rbci.angularDamping;
    friction = rbci.friction;
    //rollingFriction = rbci.rollingFriction;
    restitution = rbci.resititution;

    transformMatrix = rbci.transform;
    //localInertia = rbci.localInertia;
    collider = rbci.collisionShape;

    //m_enableGravity = rbci.enableGravity;

    //m_obb = m_collisionShape->GetLocalOBB();
    //m_obb.aabb.body = this;
    //UpdateInterpolationTransform(m_transform);

    if (mass == 0)
        inverseInertiaTensor = Matrix::Identity;
    else
        inverseInertiaTensor = collider->getTensor(mass).Invert();
}

void RigidBody::integrate(double duration)
{
    //if (!isAwake) return;

    // Calculate linear acceleration from force inputs.
    //lastFrameAcceleration = acceleration;
    //lastFrameAcceleration += forceAccum * inverseMass;

    // Calculate angular acceleration from torque inputs.
    Vector3 angularAcceleration = Vector3::Transform(torqueAccum, inverseInertiaTensorWorld);

    // Adjust velocities
    // Update linear velocity from both acceleration and impulse.
    //velocity += lastFrameAcceleration * duration;

    // Update angular velocity from both acceleration and impulse.
    //rotation += angularAcceleration * duration;

    // Impose drag.
    //velocity *= pow(linearDamping, duration);
    //rotation *= pow(angularDamping, duration);

    // Adjust positions
    // Update linear position.
    //position += velocity * duration;

    // Update angular position.
    //orientation = orientation + rotation * duration;

    // Normalise the orientation, and update the matrices with the new
    // position and orientation
    //calculateDerivedData();

    // Clear accumulators.
    clearAccumulators();

    // Update the kinetic energy store, and possibly put the body to
    // sleep.
    /*
    if (canSleep) {
        double currentMotion = velocity.scalarProduct(velocity) +
            rotation.scalarProduct(rotation);

        double bias = double_pow(0.5, duration);
        motion = bias * motion + (1 - bias)*currentMotion;

        if (motion < sleepEpsilon) setAwake(false);
        else if (motion > 10 * sleepEpsilon) motion = 10 * sleepEpsilon;
    }
    */
}

void RigidBody::setMass(const double mass)
{
    assert(mass != 0);
    RigidBody::inverseMass = ((double)1.0) / mass;
}

double RigidBody::getMass() const
{
    if (inverseMass == 0) {
        return DBL_MAX;
    }
    else {
        return ((double)1.0) / inverseMass;
    }
}

void RigidBody::setInverseMass(const double inverseMass)
{
    RigidBody::inverseMass = inverseMass;
}

double RigidBody::getInverseMass() const
{
    return inverseMass;
}

bool RigidBody::hasFiniteMass() const
{
    return inverseMass >= 0.0f;
}

void RigidBody::setInertiaTensor(const Matrix &inertiaTensor)
{
    inverseInertiaTensor = inertiaTensor.Invert();
    //_checkInverseInertiaTensor(inverseInertiaTensor);
}

Matrix RigidBody::getInertiaTensor() const
{    
    return inverseInertiaTensor.Invert();
}

Matrix RigidBody::getInertiaTensorWorld() const
{
    return inverseInertiaTensorWorld.Invert();
}

void RigidBody::setInverseInertiaTensor(const Matrix &inverseInertiaTensor)
{
    //_checkInverseInertiaTensor(inverseInertiaTensor);
    RigidBody::inverseInertiaTensor = inverseInertiaTensor;
}

Matrix RigidBody::getInverseInertiaTensor() const
{
    return inverseInertiaTensor;
}

Matrix RigidBody::getInverseInertiaTensorWorld() const
{
    return inverseInertiaTensorWorld;
}

float RigidBody::getFriction() const
{
    return friction;
}

float RigidBody::getRestitution() const
{
    return restitution;
}

void RigidBody::setDamping(const double linearDamping,
    const double angularDamping)
{
    RigidBody::linearDamping = linearDamping;
    RigidBody::angularDamping = angularDamping;
}

void RigidBody::setLinearDamping(const double linearDamping)
{
    RigidBody::linearDamping = linearDamping;
}

double RigidBody::getLinearDamping() const
{
    return linearDamping;
}

void RigidBody::setAngularDamping(const double angularDamping)
{
    RigidBody::angularDamping = angularDamping;
}

double RigidBody::getAngularDamping() const
{
    return angularDamping;
}


Transform RigidBody::getTransform() const
{
    return transformMatrix;
}

Vector3 RigidBody::getPointInLocalSpace(const Vector3 &worldPoint) const
{
    return transformMatrix.getInverse() * worldPoint;
}

Vector3 RigidBody::getPointInWorldSpace(const Vector3 &localPoint) const
{
    Vector3 result = transformMatrix * localPoint;
    return result;
}

Vector3 RigidBody::getDirectionInLocalSpace(const Vector3 &worldDirection) const
{
    Quaternion invQuaternion;
    transformMatrix.getOrientation().Inverse(invQuaternion);

    return invQuaternion * worldDirection;
}

Vector3 RigidBody::getDirectionInWorldSpace(const Vector3 &localDirection) const
{
    return transformMatrix.getOrientation() * localDirection;
}


void RigidBody::setLinearVelocity(const Vector3 &velocity)
{
    linearVelocity = velocity;
}

Vector3 RigidBody::getLinearVelocity() const
{
    return linearVelocity;
}

void RigidBody::addLinearVelocity(const Vector3 &deltaLinearVelocity)
{
    linearVelocity += deltaLinearVelocity;
}

void RigidBody::setAngularVelocity(const Vector3 &velocity)
{
    angularVelocity = velocity;
}

Vector3 RigidBody::getAngularVelocity() const
{
    return angularVelocity;
}

void RigidBody::addAngularVelocity(const Vector3 &deltaRotation)
{
    angularVelocity += deltaRotation;
}

/*
void RigidBody::setAwake(const bool awake)
{
    if (awake) {
        isAwake = true;

        // Add a bit of motion to avoid it falling asleep immediately.
        motion = sleepEpsilon * 2.0f;
    }
    else {
        isAwake = false;
        velocity.clear();
        rotation.clear();
    }
}

void RigidBody::setCanSleep(const bool canSleep)
{
    RigidBody::canSleep = canSleep;

    if (!canSleep && !isAwake) setAwake();
}
*/


void RigidBody::clearAccumulators()
{
    forceAccum = Vector3::Zero;
    torqueAccum = Vector3::Zero;
}

void RigidBody::addForce(const Vector3 &force)
{
    forceAccum += force;
    //isAwake = true;
}

void RigidBody::addTorque(const Vector3 &torque)
{
    torqueAccum += torque;
    //isAwake = true;
}

void RigidBody::setAcceleration(const Vector3 &acceleration)
{
    RigidBody::acceleration = acceleration;
}

Vector3 RigidBody::getAcceleration() const
{
    return acceleration;
}

Vector3 RigidBody::getSupportPoint(Vector3 direction) const
{
    return collider->getSupportPoint(direction);
}

void RigidBody::applyImpulse(const Vector3 &impulse)
{
    setLinearVelocity(linearVelocity + impulse * inverseMass);
}

void RigidBody::applyTorqueImpulse(const Vector3 &torque)
{
    setAngularVelocity(angularVelocity + Vector3(DirectX::XMVector3Transform(torque, inverseInertiaTensor)));
}