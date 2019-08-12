#include "pch.h"
#include "CollisionNarrow.h"

bool GJK::CollisionDetection(CollisionPrimitive *col1, CollisionPrimitive *col2)
{
    Vector3 a, b, c, d;
    Vector3 searchDirection = col1->pos - col2->pos;
    c = Support(col1, col2, searchDirection);

    searchDirection = -c;
    b = Support(col1, col2, searchDirection);

    if (b.Dot(searchDirection) < 0)
    {
        return false;
    }

    searchDirection = ((c - b).Cross(-b)).Cross(c - b);
    if (searchDirection == Vector3::Zero) 
    {
        searchDirection = (c - b).Cross(Vector3(1, 0, 0));
        if (searchDirection == Vector3::Zero)
        {
            searchDirection = (c - b).Cross(Vector3(0, 0, -1));
        }
    }

    int simplex = 2;

    for (int i = 0; i < iterations; i++)
    {
        a = Support(col1, col2, searchDirection);
        if (a.Dot(searchDirection) < 0)
        {
            return false;
        }

        simplex++;
        if (simplex == 3)
        {
            UpdateSimplex3(a, b, c, d, simplex, searchDirection);
        }
        else if(UpdateSimplex4(a, b, c, d, simplex, searchDirection))
        {
            return true;
        }
    }
    return false;
}

Vector3 GJK::Support(CollisionPrimitive *col1, CollisionPrimitive *col2, const Vector3 &dir)
{
    Vector3 p1 = col1->getFarthestPointInDirection(dir);
    Vector3 p2 = col2->getFarthestPointInDirection(Vector3(-dir.x, -dir.y, -dir.z));

    //p2 -p1
    Vector3 p3 = p1 - p2;

    return  p3;
}

void GJK::UpdateSimplex3(Vector3 &a, Vector3 &b, Vector3 &c, Vector3 &d, int &simplex, Vector3 &searchDirection)
{
    Vector3 n = (b - a).Cross(c - a);
    Vector3 ao = -a;

    simplex = 2;
    // close to edge ab
    if (((b - a).Cross(n)).Dot(ao) > 0)
    {
        c = a;
        searchDirection = ((b - a).Cross(ao)).Cross(b - a);
        return;
    }
    // close to edge ac
    if (((n).Cross(c - a)).Dot(ao) > 0)
    {
        b = a;
        searchDirection = ((c - a).Cross(ao)).Cross(c - a);
        return;
    }

    simplex = 3;
    if (n.Dot(ao) > 0)
    {
        d = c;
        c = b;
        b = a;
        searchDirection = n;
    }
    d = b;
    b = a;
    searchDirection = -n;
    return;
}

bool GJK::UpdateSimplex4(Vector3 &a, Vector3 &b, Vector3 &c, Vector3 &d, int &simplex, Vector3 &searchDirection)
{
    Vector3 ABC = (b - a).Cross(c - a);
    Vector3 ACD = (c - a).Cross(d - a);
    Vector3 ADB = (d - a).Cross(b - a);

    Vector3 ao = -a;
    simplex = 3;

    if (ABC.Dot(ao) > 0)
    {
        d = c;
        c = b;
        b = a;
        searchDirection = ABC;
        return false;
    }
    if (ACD.Dot(ao) > 0)
    {
        d = d;
        c = c;
        b = a;
        searchDirection = ACD;
        return false;
    }
    if (ADB.Dot(ao) > 0)
    {
        d = b;
        c = d;        
        b = a;
        searchDirection = ADB;
        return false;
    }

    return true;
}