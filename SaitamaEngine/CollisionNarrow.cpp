#include "pch.h"
#include "CollisionNarrow.h"

bool GJK::CollisionDetection(AABB *a, AABB *b)
{


}

Vector3 GJK::Support(AABB *a, AABB *b, const Vector3 &dir)
{
    Vector3 p1 = a.getFarthestPointInDirection(dir);
    Vector3 p2 = b.getFarthestPointInDirection(Vector3(-dir.x, -dir.y, -dir.z));

    //p2 -p1
    Vector3 p3 = p1 - p2;

    return  p3;
}

void GJK::UpdateSimplex3(Vector3 &a, Vector3 &b, Vector3 &c, Vector3 &d, int &simplex, Vector3 &searchDirection)
{

}

bool GJK::UpdateSimplex4(Vector3 &a, Vector3 &b, Vector3 &c, Vector3 &d, int &simplex, Vector3 &searchDirection)
{


    return true;
}