#pragma once
#include "Saitama.h"
#include "PhysicsDef.h"



class CollisionPrimitive
{
public:
    Vector3 pos;

    Matrix    matRS;          //rotation/scale component of model matrix
    Matrix    matRS_inverse;
    virtual Vector3 getFarthestPointInDirection(Vector3 dir) = 0;

    virtual Matrix getTensor(float mass) = 0;
    

    PhysicsDef::CollisionType getType() const;
    virtual Vector3 getSupportPoint(const Vector3 &dir) const = 0;

protected:
    PhysicsDef::CollisionType m_type;
};

class CollisionBox : public CollisionPrimitive
{
public:

    CollisionBox(const Vector3 &extents);
    /**
     * Holds the half-sizes of the box along each of its local axes.
     */
    Vector3 halfSize;

    //used for aabb test
    Vector3 max;
    Vector3 min;

    virtual Matrix getTensor(float mass) override;

    virtual Vector3 getFarthestPointInDirection(Vector3 direction) override;
    

    Vector3 getSupportPoint(const Vector3 &dir) const override;

private:
    Vector3 m_extents;
};

class CollisionSphere : public CollisionPrimitive
{
public:

    CollisionSphere(float radius);

    virtual Matrix getTensor(float mass) override;

    virtual Vector3 getFarthestPointInDirection(Vector3 direction) override { return Vector3::Zero; }


    Vector3 getSupportPoint(const Vector3 &dir) const override;

private:
    float m_radius;
};