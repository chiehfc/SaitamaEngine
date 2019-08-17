#pragma once
#include "Saitama.h"
#include "CollisionBroad.h"
#include "CollisionPrimitive.h"

struct SupportPoint
{
    Vector3 position;
    Vector3 pointA;
    Vector3 pointB;
    Vector3 direction;
};

class GJK
{
public:
    
    bool CollisionDetection(CollisionPrimitive *a, CollisionPrimitive *b, Vector3 *mtv);

private:    

    Vector3 Support(CollisionPrimitive *a, CollisionPrimitive *b, const Vector3 &dir);
    //Vector3 a, b, c, d;

    //int simplex;
    int iterations = 50;

    void UpdateSimplex3(Vector3 &a, Vector3 &b, Vector3 &c, Vector3 &d, int &simplex, Vector3 &searchDirection);
    bool UpdateSimplex4(Vector3 &a, Vector3 &b, Vector3 &c, Vector3 &d, int &simplex, Vector3 &searchDirection);

    ContactData EPA(Vector3 &a, Vector3 &b, Vector3 &c, Vector3 &d, CollisionPrimitive *collider1, CollisionPrimitive *collider2);


};