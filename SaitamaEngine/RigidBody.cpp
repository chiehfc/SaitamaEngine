#include "pch.h"
#include "RigidBody.h"

static inline void _transformInertiaTensor(Matrix &iitWorld,
    const Quaternion &q,
    const Matrix &iitBody,
    const Matrix &rotmat)
{
    double t4 = 
        rotmat._11 * iitBody._11 +
        rotmat._12 * iitBody._21 +
        rotmat._13 * iitBody._31;
    double t9 = 
        rotmat._11 * iitBody._12 +
        rotmat._12 * iitBody._22 +
        rotmat._13 * iitBody._32;
    double t14 = 
        rotmat._11 * iitBody._13 +
        rotmat._12 * iitBody._23 +
        rotmat._13 * iitBody._33;
    double t28 = 
        rotmat._21 * iitBody._11 +
        rotmat._22 * iitBody._21 +
        rotmat._23 * iitBody._31;
    double t33 = 
        rotmat._21 * iitBody._12 +
        rotmat._22 * iitBody._22 +
        rotmat._23 * iitBody._32;
    double t38 = 
        rotmat._21 * iitBody._13 +
        rotmat._22 * iitBody._23 +
        rotmat._23 * iitBody._33;
    double t52 = 
        rotmat._31 * iitBody._11 +
        rotmat._32 * iitBody._21 +
        rotmat._33 * iitBody._31;
    double t57 = 
        rotmat._31 * iitBody._12 +
        rotmat._32 * iitBody._22 +
        rotmat._33 * iitBody._32;
    double t62 = 
        rotmat._31 * iitBody._13 +
        rotmat._32 * iitBody._23 +
        rotmat._33 * iitBody._33;

    iitWorld._11 = 
        t4 * rotmat._11 +
        t9 * rotmat._12 +
        t14 * rotmat._13;
    iitWorld._12 =
        t4 * rotmat._21 +
        t9 * rotmat._22 +
        t14 * rotmat._23;
    iitWorld._13 = 
        t4 * rotmat._31 +
        t9 * rotmat._32 +
        t14 * rotmat._33;
    iitWorld._21 = 
        t28 * rotmat._11 +
        t33 * rotmat._12 +
        t38 * rotmat._13;
    iitWorld._22 = 
        t28 * rotmat._21 +
        t33 * rotmat._22 +
        t38 * rotmat._23;
    iitWorld._23 = 
        t28 * rotmat._31 +
        t33 * rotmat._32 +
        t38 * rotmat._33;
    iitWorld._31 = 
        t52 * rotmat._11 +
        t57 * rotmat._12 +
        t62 * rotmat._13;
    iitWorld._32 = 
        t52 * rotmat._21 +
        t57 * rotmat._22 +
        t62 * rotmat._23;
    iitWorld._33 = 
        t52 * rotmat._31 +
        t57 * rotmat._32 +
        t62 * rotmat._33;
}

static inline void _calculateTransformMatrix(Matrix &transformMatrix,
    const Vector3 &position,
    const Quaternion &orientation)
{
    transformMatrix._11 = 1 - 2 * orientation.y*orientation.y -
        2 * orientation.z*orientation.z;
    transformMatrix._12 = 2 * orientation.x*orientation.y -
        2 * orientation.w*orientation.z;
    transformMatrix._13 = 2 * orientation.x*orientation.z +
        2 * orientation.w*orientation.y;
    transformMatrix._14 = position.x;

    transformMatrix._21 = 2 * orientation.x*orientation.y +
        2 * orientation.w*orientation.z;
    transformMatrix._22 = 1 - 2 * orientation.x*orientation.x -
        2 * orientation.z*orientation.z;
    transformMatrix._23 = 2 * orientation.y*orientation.z -
        2 * orientation.w*orientation.x;
    transformMatrix._24 = position.y;

    transformMatrix._31 = 2 * orientation.x*orientation.z -
        2 * orientation.w*orientation.y;
    transformMatrix._32 = 2 * orientation.y*orientation.z +
        2 * orientation.w*orientation.x;
    transformMatrix._33 = 1 - 2 * orientation.x*orientation.x -
        2 * orientation.y*orientation.y;
    transformMatrix._34 = position.z;
}

void RigidBody::calculateDerivedData()
{
    orientation.Normalize();

    // Calculate the transform matrix for the body.
    _calculateTransformMatrix(transformMatrix, position, orientation);

    // Calculate the inertiaTensor in world space.
    _transformInertiaTensor(inverseInertiaTensorWorld,
        orientation,
        inverseInertiaTensor,
        transformMatrix);

    // Update collider position
    // TODO: Not sure if this is the right way to do, do we really need to update collider?
    collider->pos = position;
}

void RigidBody::integrate(double duration)
{
    //if (!isAwake) return;

    // Calculate linear acceleration from force inputs.
    lastFrameAcceleration = acceleration;
    lastFrameAcceleration += forceAccum * inverseMass;

    // Calculate angular acceleration from torque inputs.
    Vector3 angularAcceleration = Vector3::Transform(torqueAccum, inverseInertiaTensorWorld);

    // Adjust velocities
    // Update linear velocity from both acceleration and impulse.
    velocity += lastFrameAcceleration * duration;

    // Update angular velocity from both acceleration and impulse.
    rotation += angularAcceleration * duration;

    // Impose drag.
    velocity *= pow(linearDamping, duration);
    rotation *= pow(angularDamping, duration);

    // Adjust positions
    // Update linear position.
    position += velocity * duration;

    // Update angular position.
    orientation = orientation + rotation * duration;

    // Normalise the orientation, and update the matrices with the new
    // position and orientation
    calculateDerivedData();

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

void RigidBody::getInertiaTensor(Matrix *inertiaTensor) const
{
    inertiaTensor = &inverseInertiaTensor.Invert();
    //inertiaTensor->setInverse(inverseInertiaTensor);
}

Matrix RigidBody::getInertiaTensor() const
{
    Matrix it;
    getInertiaTensor(&it);
    return it;
}

void RigidBody::getInertiaTensorWorld(Matrix *inertiaTensor) const
{
    inertiaTensor = &inverseInertiaTensorWorld.Invert();
}

Matrix RigidBody::getInertiaTensorWorld() const
{
    Matrix it;
    getInertiaTensorWorld(&it);
    return it;
}

void RigidBody::setInverseInertiaTensor(const Matrix &inverseInertiaTensor)
{
    //_checkInverseInertiaTensor(inverseInertiaTensor);
    RigidBody::inverseInertiaTensor = inverseInertiaTensor;
}

void RigidBody::getInverseInertiaTensor(Matrix *inverseInertiaTensor) const
{
    *inverseInertiaTensor = RigidBody::inverseInertiaTensor;
}

Matrix RigidBody::getInverseInertiaTensor() const
{
    return inverseInertiaTensor;
}

void RigidBody::getInverseInertiaTensorWorld(Matrix *inverseInertiaTensor) const
{
    *inverseInertiaTensor = inverseInertiaTensorWorld;
}

Matrix RigidBody::getInverseInertiaTensorWorld() const
{
    return inverseInertiaTensorWorld;
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

void RigidBody::setPosition(const Vector3 &position)
{
    RigidBody::position = position;
}

void RigidBody::setPosition(const double x, const double y, const double z)
{
    position.x = x;
    position.y = y;
    position.z = z;
}

void RigidBody::getPosition(Vector3 *position) const
{
    *position = RigidBody::position;
}

Vector3 RigidBody::getPosition() const
{
    return position;
}

void RigidBody::setOrientation(const Quaternion &orientation)
{
    RigidBody::orientation = orientation;
    RigidBody::orientation.Normalize();
}

void RigidBody::setOrientation(const double r, const double i,
    const double j, const double k)
{
    orientation.w = r;
    orientation.x = i;
    orientation.y = j;
    orientation.z = k;
    orientation.Normalize();
}

void RigidBody::getOrientation(Quaternion *orientation) const
{
    *orientation = RigidBody::orientation;
}

Quaternion RigidBody::getOrientation() const
{
    return orientation;
}

void RigidBody::getOrientation(Matrix *matrix) const
{
    getOrientation(matrix);
}

void RigidBody::getOrientation(double matrix[9]) const
{
    matrix[0] = transformMatrix._11;
    matrix[1] = transformMatrix._12;
    matrix[2] = transformMatrix._13;

    matrix[3] = transformMatrix._21;
    matrix[4] = transformMatrix._22;
    matrix[5] = transformMatrix._23;

    matrix[6] = transformMatrix._31;
    matrix[7] = transformMatrix._32;
    matrix[8] = transformMatrix._33;
}

void RigidBody::getTransform(Matrix *transform) const
{
    memcpy(transform, &transformMatrix, sizeof(Matrix));
}

void RigidBody::getTransform(double matrix[16]) const
{
    memcpy(matrix, &transformMatrix, sizeof(double) * 12);
    matrix[12] = matrix[13] = matrix[14] = 0;
    matrix[15] = 1;
}

void RigidBody::getGLTransform(float matrix[16]) const
{
    matrix[0] = (float)transformMatrix._11;
    matrix[1] = (float)transformMatrix._21;
    matrix[2] = (float)transformMatrix._31;
    matrix[3] = 0;

    matrix[4] = (float)transformMatrix._12;
    matrix[5] = (float)transformMatrix._22;
    matrix[6] = (float)transformMatrix._32;
    matrix[7] = 0;

    matrix[8] = (float)transformMatrix._13;
    matrix[9] = (float)transformMatrix._23;
    matrix[10] = (float)transformMatrix._33;
    matrix[11] = 0;

    matrix[12] = (float)transformMatrix._14;
    matrix[13] = (float)transformMatrix._24;
    matrix[14] = (float)transformMatrix._34;
    matrix[15] = 1;
}

Matrix RigidBody::getTransform() const
{
    return transformMatrix;
}


Vector3 RigidBody::getPointInLocalSpace(const Vector3 &point) const
{
    Vector3 tmp = point;
    tmp.x -= transformMatrix._14;
    tmp.y -= transformMatrix._24;
    tmp.z -= transformMatrix._34;
    return Vector3::Transform(tmp, transformMatrix.Transpose());
}

Vector3 RigidBody::getPointInWorldSpace(const Vector3 &point) const
{
    return Vector3::Transform(point, transformMatrix);
}

Vector3 RigidBody::getDirectionInLocalSpace(const Vector3 &direction) const
{
    return Vector3::Transform(direction, transformMatrix.Transpose());
}

Vector3 RigidBody::getDirectionInWorldSpace(const Vector3 &direction) const
{
    return Vector3::Transform(direction, transformMatrix);
}


void RigidBody::setVelocity(const Vector3 &velocity)
{
    RigidBody::velocity = velocity;
}

void RigidBody::setVelocity(const double x, const double y, const double z)
{
    velocity.x = x;
    velocity.y = y;
    velocity.z = z;
}

void RigidBody::getVelocity(Vector3 *velocity) const
{
    *velocity = RigidBody::velocity;
}

Vector3 RigidBody::getVelocity() const
{
    return velocity;
}

void RigidBody::addVelocity(const Vector3 &deltaVelocity)
{
    velocity += deltaVelocity;
}

void RigidBody::setRotation(const Vector3 &rotation)
{
    RigidBody::rotation = rotation;
}

void RigidBody::setRotation(const double x, const double y, const double z)
{
    rotation.x = x;
    rotation.y = y;
    rotation.z = z;
}

void RigidBody::getRotation(Vector3 *rotation) const
{
    *rotation = RigidBody::rotation;
}

Vector3 RigidBody::getRotation() const
{
    return rotation;
}

void RigidBody::addRotation(const Vector3 &deltaRotation)
{
    rotation += deltaRotation;
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

void RigidBody::getLastFrameAcceleration(Vector3 *acceleration) const
{
    *acceleration = lastFrameAcceleration;
}

Vector3 RigidBody::getLastFrameAcceleration() const
{
    return lastFrameAcceleration;
}

void RigidBody::clearAccumulators()
{
    forceAccum = Vector3::Zero;
    torqueAccum = Vector3::Zero;
}

void RigidBody::addForce(const Vector3 &force)
{
    forceAccum += force;
    isAwake = true;
}

void RigidBody::addForceAtBodyPoint(const Vector3 &force,
    const Vector3 &point)
{
    // Convert to coordinates relative to center of mass.
    Vector3 pt = getPointInWorldSpace(point);
    addForceAtPoint(force, pt);

}

void RigidBody::addForceAtPoint(const Vector3 &force,
    const Vector3 &point)
{
    // Convert to coordinates relative to center of mass.
    Vector3 pt = point;
    pt -= position;

    forceAccum += force;
    torqueAccum += pt.Cross(force);

    isAwake = true;
}

void RigidBody::addTorque(const Vector3 &torque)
{
    torqueAccum += torque;
    isAwake = true;
}

void RigidBody::setAcceleration(const Vector3 &acceleration)
{
    RigidBody::acceleration = acceleration;
}

void RigidBody::setAcceleration(const double x, const double y, const double z)
{
    acceleration.x = x;
    acceleration.y = y;
    acceleration.z = z;
}

void RigidBody::getAcceleration(Vector3 *acceleration) const
{
    *acceleration = RigidBody::acceleration;
}

Vector3 RigidBody::getAcceleration() const
{
    return acceleration;
}
