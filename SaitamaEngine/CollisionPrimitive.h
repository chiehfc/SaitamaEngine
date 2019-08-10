#pragma once
#include "Saitama.h"
#include "RigidBody.h"

class CollisionPrimitive
{
public:

    RigidBody *body;

    //Matrix offset;

    //void calculateInternals();

    const Matrix &getTransform() const
    {
        return transform;
    }

    

protected:

    Matrix transform;
};

class CollisionBox : public CollisionPrimitive
{
public:
    /**
     * Holds the half-sizes of the box along each of its local axes.
     */
    Vector3 halfSize;

    //used for aabb test
    Vector3 max;
    Vector3 min;

    Vector3 getFarthestPointInDirection(const Vector3 &direction)
    {
        Vector3 returnVector;

        body->getTransform();
    }
};