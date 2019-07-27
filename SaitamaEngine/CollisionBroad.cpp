#include "pch.h"
#include "CollisionBroad.h"

BoundingSphere::BoundingSphere(const Vector3 &center, double radius)
{
    BoundingSphere::center = center;
    BoundingSphere::radius = radius;
}

BoundingSphere::BoundingSphere(const BoundingSphere &one, const BoundingSphere &two)
{
    Vector3 centerOffset = two.center - one.center;
    double distance = Vector3::DistanceSquared(two.center, one.center);
    double radiusDiff = two.radius - one.radius;

    // large sphere enclose small one
    if (radiusDiff * radiusDiff >= distance)
    {
        if (one.radius > two.radius)
        {
            center = one.center;
            radius = one.radius;
        } 
        else
        {
            center = two.center;
            radius = two.radius;
        }
    }
    else
    {
        distance = Vector3::Distance(one.center, two.center);
        radius = (distance + one.radius + two.radius) * 0.5;

        center = one.center;
        if (distance > 0)
        {
            center += centerOffset * ((radius - one.radius) / distance);
        }
    }
}

bool BoundingSphere::overlaps(const BoundingSphere *other) const
{
    double distanceSquared = Vector3::DistanceSquared(center, other->center);
    return distanceSquared < (radius + other->radius) * (radius + other->radius);
}

double BoundingSphere::getGrowth(const BoundingSphere &other) const
{
    BoundingSphere newSphere(*this, other);

    return newSphere.radius * newSphere.radius - radius * radius;
}