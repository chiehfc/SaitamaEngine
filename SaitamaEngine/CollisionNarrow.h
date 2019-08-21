#pragma once
#include "Saitama.h"
#include "PhysicsDef.h"
#include "CollisionBroad.h"
#include "RigidBody.h"
#include "Manifold.h"

struct Face
{
    PhysicsDef::SupportPoint a;
    PhysicsDef::SupportPoint b;
    PhysicsDef::SupportPoint c;
    Vector3 normal;
};

struct Edge
{
    PhysicsDef::SupportPoint a;
    PhysicsDef::SupportPoint b;
};

class GJK
{
public:
    
    bool CollisionDetection(RigidBody *a, RigidBody *b, Manifold &manifold);

private:    

    PhysicsDef::SupportPoint Support(RigidBody *a, RigidBody *b, const Vector3 &dir);
    //Vector3 a, b, c, d;

    //int simplex;
    int iterations = 50;

    void UpdateSimplex3(PhysicsDef::SupportPoint &a, PhysicsDef::SupportPoint &b, PhysicsDef::SupportPoint &c, PhysicsDef::SupportPoint &d, int &simplex, Vector3 &searchDirection);
    bool UpdateSimplex4(PhysicsDef::SupportPoint &a, PhysicsDef::SupportPoint &b, PhysicsDef::SupportPoint &c, PhysicsDef::SupportPoint &d, int &simplex, Vector3 &searchDirection);

    PhysicsDef::ContactData EPA(PhysicsDef::SupportPoint &a, PhysicsDef::SupportPoint &b, PhysicsDef::SupportPoint &c, PhysicsDef::SupportPoint &d, RigidBody *collider1, RigidBody *collider2);


};