#pragma once
#include "Saitama.h"
#include "CollisionBroad.h"
#include "CollisionPrimitive.h"

class GJK
{
public:
    
    bool CollisionDetection(CollisionPrimitive *a, CollisionPrimitive *b);

private:    

    Vector3 Support(CollisionPrimitive *a, CollisionPrimitive *b, const Vector3 &dir);
    //Vector3 a, b, c, d;

    //int simplex;
    int iterations = 50;

    void UpdateSimplex3(Vector3 &a, Vector3 &b, Vector3 &c, Vector3 &d, int &simplex, Vector3 &searchDirection);
    bool UpdateSimplex4(Vector3 &a, Vector3 &b, Vector3 &c, Vector3 &d, int &simplex, Vector3 &searchDirection);
};