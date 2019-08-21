#include "pch.h"
#include "CollisionNarrow.h"

using namespace PhysicsDef;

static const float EPA_TOLERANCE = 0.0001;
static const int EPA_MAX_FACES = 64;
static const int EPA_MAX_LOOSE_EDGES = 32;
static const int EPA_MAX_ITERATIONS = 64;

bool GJK::CollisionDetection(RigidBody *body1, RigidBody *body2, Manifold &manifold)
{
    SupportPoint a, b, c, d;
    Vector3 searchDirection = body1->getTransform().getPosition() - body2->getTransform().getPosition();

    c = Support(body1, body2, searchDirection);

    //searchDirection = -c;
    searchDirection = -c.position;
    
    b = Support(body1, body2, searchDirection);

    if (b.position.Dot(searchDirection) < 0)
    {
        return false;
    }

    searchDirection = ((c.position - b.position).Cross(-b.position)).Cross(c.position - b.position);
    if (searchDirection == Vector3::Zero) 
    {
        searchDirection = (c.position - b.position).Cross(Vector3(1, 0, 0));
        if (searchDirection == Vector3::Zero)
        {
            searchDirection = (c.position - b.position).Cross(Vector3(0, 0, -1));
        }
    }

    int simplex = 2;

    for (int i = 0; i < iterations; i++)
    {
        a = Support(body1, body2, searchDirection);
        if (a.position.Dot(searchDirection) < 0)
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
            ContactData contact = EPA(a, b, c, d, body1, body2);
            manifold.update(&contact, 1);
            
            return true;
        }
    }
    return false;
}

SupportPoint GJK::Support(RigidBody *body1, RigidBody *body2, const Vector3 &dir)
{
    SupportPoint result;
    result.originA = body1->getPointInWorldSpace(body1->getSupportPoint(dir));
    result.originB = body2->getPointInWorldSpace(body2->getSupportPoint(-dir));
    result.position = result.originA - result.originB;
    result.direction = dir;

    return result;
}

void GJK::UpdateSimplex3(SupportPoint &a, SupportPoint &b, SupportPoint &c, SupportPoint &d, int &simplex, Vector3 &searchDirection)
{
    Vector3 n = (b.position - a.position).Cross(c.position - a.position);
    Vector3 ao = -a.position;

    simplex = 2;
    // close to edge ab
    if (((b.position - a.position).Cross(n)).Dot(ao) > 0)
    {
        c = a;
        searchDirection = ((b.position - a.position).Cross(ao)).Cross(b.position - a.position);
        return;
    }
    // close to edge ac
    if (((n).Cross(c.position - a.position)).Dot(ao) > 0)
    {
        b = a;
        searchDirection = ((c.position - a.position).Cross(ao)).Cross(c.position - a.position);
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

bool GJK::UpdateSimplex4(SupportPoint &a, SupportPoint &b, SupportPoint &c, SupportPoint &d, int &simplex, Vector3 &searchDirection)
{
    Vector3 ABC = (b.position - a.position).Cross(c.position - a.position);
    Vector3 ACD = (c.position - a.position).Cross(d.position - a.position);
    Vector3 ADB = (d.position - a.position).Cross(b.position - a.position);

    Vector3 ao = -a.position;
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

ContactData GJK::EPA(SupportPoint &a, SupportPoint &b, SupportPoint &c, SupportPoint &d, RigidBody *body1, RigidBody *body2)
{
    ContactData result;
    Face faces[EPA_MAX_FACES]; // 3 vertex and 1 normal facing out

    faces[0].a = a;
    faces[0].b = b;
    faces[0].c = c;
    ((b.position - a.position).Cross(c.position - a.position)).Normalize(faces[0].normal);
    faces[1].a = a;
    faces[1].b = c;
    faces[1].c = d;
    ((c.position - a.position).Cross(d.position - a.position)).Normalize(faces[1].normal);
    faces[2].a = a;
    faces[2].b = d;
    faces[2].c = b;
    ((d.position - a.position).Cross(b.position - a.position)).Normalize(faces[2].normal);
    faces[3].a = b;
    faces[3].b = d;
    faces[3].c = c;
    ((d.position - b.position).Cross(c.position - b.position)).Normalize(faces[3].normal);

    int num_faces = 4;
    int closestFace = 0;

    for (int iter = 0; iter < EPA_MAX_ITERATIONS; iter++)
    {
        // Find face that's closest to the origin
        float minDistance = faces[0].a.position.Dot(faces[0].normal);
        closestFace = 0;
        for (int i = 1; i < num_faces; i++)
        {
            float dist = faces[i].a.position.Dot(faces[i].normal);
            if (dist < minDistance)
            {
                minDistance = dist;
                closestFace = i;
            }
        }

        // Search normal to face that's closest to origin
        Vector3 searchDirection = faces[closestFace].normal;
        SupportPoint p = Support(body1, body2, searchDirection);

        if ((p.position.Dot(searchDirection) - minDistance) < EPA_TOLERANCE)
        {
            // Convergence
            float P_dot_searchDirection = p.position.Dot(searchDirection);
            Vector3 mtv = Vector3(faces[closestFace].normal.x * P_dot_searchDirection,
                faces[closestFace].normal.y * P_dot_searchDirection,
                faces[closestFace].normal.z * P_dot_searchDirection
            );
            /*result.normal = Vector3(faces[closestFace].normal.x * P_dot_searchDirection,
                faces[closestFace].normal.y * P_dot_searchDirection,
                faces[closestFace].normal.z * P_dot_searchDirection
            );*/
            
            result.normal = searchDirection;
            result.depth = minDistance;
            break;
        }

        Edge looseEdges[EPA_MAX_LOOSE_EDGES]; // keep track of loose that need to fix after removing faces
        int num_loose_edges = 0;
        
        // Find all triangle can be seen by p
        for (int i = 0; i < num_faces; i++)
        {
            if (faces[i].normal.Dot(p.position - faces[i].a.position) > 0)
            {
                for (int j = 0; j < 3; j++)
                {
                    Edge currentEdge;
                    if (j == 0) {
                        currentEdge.a = faces[i].a;
                        currentEdge.b = faces[i].b;
                    } else if (j == 1) {
                        currentEdge.a = faces[i].b;
                        currentEdge.b = faces[i].c;
                    } else if (j == 2) {
                        currentEdge.a = faces[i].c;
                        currentEdge.b = faces[i].a;
                    }
                    bool found_edge = false;
                    for (int k = 0; k < num_loose_edges; k++)
                    {
                        // Is current edge equal to current in reverse
                        if (looseEdges[k].b.position == currentEdge.a.position && looseEdges[k].a.position == currentEdge.b.position)
                        {
                            looseEdges[k].a = looseEdges[num_loose_edges - 1].a;
                            looseEdges[k].b = looseEdges[num_loose_edges - 1].b;
                            num_loose_edges--;
                            found_edge = true;
                            k = num_loose_edges; // set k = num_loose_edges so we will exit loop
                        }
                    }

                    if (!found_edge) // Add current edge to edges
                    {
                        if (num_loose_edges >= EPA_MAX_LOOSE_EDGES)
                        {
                            break;
                        }
                        looseEdges[num_loose_edges].a = currentEdge.a;
                        looseEdges[num_loose_edges].b = currentEdge.b;
                        num_loose_edges++;
                    }
                }

                // Remove triangle from list
                faces[i].a = faces[num_faces - 1].a;
                faces[i].b = faces[num_faces - 1].b;
                faces[i].c = faces[num_faces - 1].c;
                faces[i].normal = faces[num_faces - 1].normal;
                num_faces--;
                i--;
            }
        }

        // Reconstruct polytope with p added
        for (int i = 0; i < num_loose_edges; i++)
        {
            if (num_faces >= EPA_MAX_FACES)
            {
                break;
            }
            faces[num_faces].a = looseEdges[i].a;
            faces[num_faces].b = looseEdges[i].b;
            faces[num_faces].c = p;
            ((looseEdges[i].a.position - looseEdges[i].b.position).Cross(looseEdges[i].a.position - p.position)).Normalize(faces[num_faces].normal);

            // Check for wrong normal to maintain Counter Clock Wise winding
            float bias = 0.000001;
            if (faces[num_faces].a.position.Dot(faces[num_faces].normal) + bias < 0)
            {
                // Swap winding
                SupportPoint temp = faces[num_faces].a;
                faces[num_faces].a = faces[num_faces].b;
                faces[num_faces].b = temp;
                faces[num_faces].normal = -faces[num_faces].normal;
            }
            num_faces++;
        }
    }
        
    //Return most recent closest point
    float projection = faces[closestFace].a.position.Dot(faces[closestFace].normal);
    Vector3 mtv = Vector3(faces[closestFace].normal.x * projection,
        faces[closestFace].normal.y * projection,
        faces[closestFace].normal.z * projection
    );

    Vector3 originProjectionPlane = -mtv;

    Vector3 v0 = faces[closestFace].b.position - faces[closestFace].a.position;
    Vector3 v1 = faces[closestFace].c.position - faces[closestFace].a.position;
    Vector3 v2 = originProjectionPlane - faces[closestFace].a.position;
    float d00 = v0.Dot(v0);
    float d01 = v0.Dot(v1);
    float d11 = v1.Dot(v1);
    float d20 = v2.Dot(v0);
    float d21 = v2.Dot(v1);
    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;

    result.globalPositionA = u * faces[closestFace].a.originA + v * faces[closestFace].b.originA + w * faces[closestFace].c.originA;
    result.globalPositionB = u * faces[closestFace].a.originB + v * faces[closestFace].b.originB + w * faces[closestFace].c.originB;

    result.localPositionA = body1->getPointInLocalSpace(result.globalPositionA);
    result.localPositionB = body2->getPointInLocalSpace(result.globalPositionB);

    return result;
}