#pragma once
#include "Saitama.h"
class CollisionPrimitive
{
public:
    Vector3 pos;

    //Matrix offset;

    //void calculateInternals();

    const Matrix &getTransform() const
    {
        return transform;
    }

    Matrix    matRS;          //rotation/scale component of model matrix
    Matrix    matRS_inverse;
    virtual Vector3 getFarthestPointInDirection(Vector3 dir) { return Vector3::Zero; }

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

    virtual Vector3 getFarthestPointInDirection(Vector3 direction) override
    {
        auto dir = Vector3::Transform(direction, matRS_inverse); //find support in model space

        Vector3 result;
        result.x = (dir.x>0) ? max.x : min.x;
        result.y = (dir.y>0) ? max.y : min.y;
        result.z = (dir.z>0) ? max.z : min.z;

        return Vector3::Transform(result, matRS_inverse); + pos; //convert support to world space
    }
};