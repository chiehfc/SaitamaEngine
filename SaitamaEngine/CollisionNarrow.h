#pragma once
#include "Saitama.h"
#include "CollisionBroad.h"

class GJK
{
public:
    
    bool CollisionDetection(AABB *a, AABB *b);

private:    

    Vector3 Support(AABB *a, AABB *b, const Vector3 &dir);
    Vector3 a, b, c, d;

    int simplex;

    void UpdateSimplex3(Vector3 &a, Vector3 &b, Vector3 &c, Vector3 &d, int &simplex, Vector3 &searchDirection);
    bool UpdateSimplex4(Vector3 &a, Vector3 &b, Vector3 &c, Vector3 &d, int &simplex, Vector3 &searchDirection);
};