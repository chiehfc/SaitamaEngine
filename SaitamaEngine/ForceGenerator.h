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


class ForceRegistry
{
protected:

    /**
    * Keeps track of one force generator and the body it
    * applies to.
    */
    struct ForceRegistration
    {
        RigidBody *body;
        ForceGenerator *fg;
    };

    /**
    * Holds the list of registrations.
    */
    typedef std::vector<ForceRegistration> Registry;
    Registry registrations;

public:
    /**
    * Registers the given force generator to apply to the
    * given body.
    */
    void add(RigidBody* body, ForceGenerator *fg);

    /**
    * Removes the given registered pair from the registry.
    * If the pair is not registered, this method will have
    * no effect.
    */
    void remove(RigidBody* body, ForceGenerator *fg);

    /**
    * Clears all registrations from the registry. This will
    * not delete the bodies or the force generators
    * themselves, just the records of their connection.
    */
    void clear();

    /**
    * Calls all the force generators to update the forces of
    * their corresponding bodies.
    */
    void updateForces(double duration);
};
