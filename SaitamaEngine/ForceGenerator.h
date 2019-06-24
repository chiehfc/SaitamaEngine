#pragma once
#include "RigidBody.h"

class ForceGenerator
{
public:

    /**
    * Overload this in implementations of the interface to calculate
    * and update the force applied to the given rigid body.
    */
    virtual void updateForce(RigidBody *body, double duration) = 0;
};

class Gravity : public ForceGenerator
{
    /** Holds the acceleration due to gravity. */
    Vector3 gravity;

public:

    /** Creates the generator with the given acceleration. */
    Gravity(const Vector3 &gravity);

    /** Applies the gravitational force to the given rigid body. */
    virtual void updateForce(RigidBody *body, double duration);
};