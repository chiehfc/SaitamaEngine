#pragma once
#include "PhysicsDef.h"
#include "RigidBody.h"

class Manifold
{
public:
    Manifold(RigidBody *a, RigidBody *b);

    void solve();
    void update(PhysicsDef::ContactData *newContacts, int newContactCount);

    static const int MIN_POINTS = 4;
    std::vector<PhysicsDef::ContactData> m_contacts;

    RigidBody *m_bodyA = nullptr;
    RigidBody *m_bodyB = nullptr;
};

//based on Box2D Lite's ArbiterKey
struct ManifoldKey
{
    ManifoldKey(RigidBody *a, RigidBody *b)
    {
        if (a < b)
        {
            bodyA = a;
            bodyB = b;
        }
        else
        {
            bodyA = b;
            bodyB = a;
        }

    }

    RigidBody *bodyA = nullptr;
    RigidBody *bodyB = nullptr;
};

inline bool operator <(const ManifoldKey& m1, const ManifoldKey& m2)
{
    if (m1.bodyA < m2.bodyA)
        return true;

    if (m1.bodyA == m2.bodyA && m1.bodyB < m2.bodyB)
        return true;

    return false;
}
