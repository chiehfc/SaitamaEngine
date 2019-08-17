#include "pch.h"
#include "CollisionNarrow.h"


static const float EPA_TOLERANCE = 0.0001;
static const int EPA_MAX_FACES = 64;
static const int EPA_MAX_LOOSE_EDGES = 32;
static const int EPA_MAX_ITERATIONS = 64;

bool GJK::CollisionDetection(CollisionPrimitive *col1, CollisionPrimitive *col2, Vector3 *mtv)
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
            if (mtv) {
                //*mtv = EPA(a, b, c, d, col1, col2);
                ContactData contact = EPA(a, b, c, d, col1, col2);
            }
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

ContactData GJK::EPA(Vector3 &a, Vector3 &b, Vector3 &c, Vector3 &d, CollisionPrimitive *collider1, CollisionPrimitive *collider2)
{
    ContactData result;
    Vector3 faces[EPA_MAX_FACES][4]; // 3 vertex and 1 normal facing out

    faces[0][0] = a;
    faces[0][1] = b;
    faces[0][2] = c;
    ((b - a).Cross(c - a)).Normalize(faces[0][3]);
    faces[1][0] = a;
    faces[1][1] = c;
    faces[1][2] = d;
    ((c - a).Cross(d - a)).Normalize(faces[1][3]);
    faces[2][0] = a;
    faces[2][1] = d;
    faces[2][2] = b;
    ((d - a).Cross(b - a)).Normalize(faces[2][3]);
    faces[3][0] = b;
    faces[3][1] = d;
    faces[3][2] = c;
    ((d - b).Cross(c - b)).Normalize(faces[3][3]);

    int num_faces = 4;
    int closestFace = 0;

    for (int iter = 0; iter < EPA_MAX_ITERATIONS; iter++)
    {
        // Find face that's closest to the origin
        float minDistance = faces[0][0].Dot(faces[0][3]);
        closestFace = 0;
        for (int i = 1; i < num_faces; i++)
        {
            float dist = faces[i][0].Dot(faces[i][3]);
            if (dist < minDistance)
            {
                minDistance = dist;
                closestFace = i;
            }
        }

        // Search normal to face that's closest to origin
        Vector3 searchDirection = faces[closestFace][3];
        Vector3 p = Support(collider1, collider2, searchDirection);

        if ((p.Dot(searchDirection) - minDistance) < EPA_TOLERANCE)
        {
            // Convergence
            float P_dot_searchDirection = p.Dot(searchDirection);
            Vector3 mtv = Vector3(faces[closestFace][3].x * P_dot_searchDirection,
                faces[closestFace][3].y * P_dot_searchDirection,
                faces[closestFace][3].z * P_dot_searchDirection
            );
            /*return Vector3(faces[closestFace][3].x * P_dot_searchDirection,
                faces[closestFace][3].y * P_dot_searchDirection,
                faces[closestFace][3].z * P_dot_searchDirection
            );*/
            result.normal = searchDirection;
            result.depth = minDistance;
            break;
        }

        Vector3 looseEdges[EPA_MAX_LOOSE_EDGES][2]; // keep track of loose that need to fix after removing faces
        int num_loose_edges = 0;
        
        // Find all triangle can be seen by p
        for (int i = 0; i < num_faces; i++)
        {
            if (faces[i][3].Dot(p - faces[i][0]) > 0) 
            {
                for (int j = 0; j < 3; j++)
                {
                    Vector3 currentEdge[2] = { faces[i][j], faces[i][(j+1) % 3] };
                    bool found_edge = false;
                    for (int k = 0; k < num_loose_edges; k++)
                    {
                        // Is current edge equal to current in reverse
                        if (looseEdges[k][1] == currentEdge[0] && looseEdges[k][0] == currentEdge[1])
                        {
                            looseEdges[k][0] = looseEdges[num_loose_edges - 1][0];
                            looseEdges[k][1] = looseEdges[num_loose_edges - 1][1];
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
                        looseEdges[num_loose_edges][0] = currentEdge[0];
                        looseEdges[num_loose_edges][1] = currentEdge[1];
                        num_loose_edges++;
                    }
                }

                // Remove triangle from list
                faces[i][0] = faces[num_faces - 1][0];
                faces[i][1] = faces[num_faces - 1][1];
                faces[i][2] = faces[num_faces - 1][2];
                faces[i][3] = faces[num_faces - 1][3];
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
            faces[num_faces][0] = looseEdges[i][0];
            faces[num_faces][1] = looseEdges[i][1];
            faces[num_faces][2] = p;
            ((looseEdges[i][0] - looseEdges[i][1]).Cross(looseEdges[i][0] - p)).Normalize(faces[num_faces][3]);

            // Check for wrong normal to maintain Counter Clock Wise winding
            float bias = 0.000001;
            if (faces[num_faces][0].Dot(faces[num_faces][3]) + bias < 0)
            {
                // Swap winding
                Vector3 temp = faces[num_faces][0];
                faces[num_faces][0] = faces[num_faces][1];
                faces[num_faces][1] = temp;
                faces[num_faces][3] = -faces[num_faces][3];
            }
            num_faces++;
        }
    }

    std::cout<< "EPA did not converge\n" << std::endl;
    //Return most recent closest point
    float projection = faces[closestFace][0].Dot(faces[closestFace][3]);
    Vector3 mtv = Vector3(faces[closestFace][3].x * projection,
        faces[closestFace][3].y * projection,
        faces[closestFace][3].z * projection
    );

    Vector3 originProjectionPlane = -mtv;

    Vector3 v0 = faces[closestFace][1] - faces[closestFace][0];
    Vector3 v1 = faces[closestFace][2] - faces[closestFace][0];
    Vector3 v2 = originProjectionPlane - faces[closestFace][0];
    float d00 = v0.Dot(v0);
    float d01 = v0.Dot(v1);
    float d11 = v1.Dot(v1);
    float d20 = v2.Dot(v0);
    float d21 = v2.Dot(v1);
    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;

    result.globalPositionA = u * (collider1->pos - collider1->getFarthestPointInDirection(faces[closestFace][0])) + v * (collider1->pos - collider1->getFarthestPointInDirection(faces[closestFace][1])) + w * (collider1->pos - collider1->getFarthestPointInDirection(faces[closestFace][2]));
    result.globalPositionB = u * (collider2->pos - collider2->getFarthestPointInDirection(faces[closestFace][0])) + v * (collider2->pos - collider2->getFarthestPointInDirection(faces[closestFace][1])) + w * (collider2->pos - collider2->getFarthestPointInDirection(faces[closestFace][2]));

    result.localPositionA = bodyAtoWorld.Inverse() * result.worldPointA;
    result.localPositionB = bodyBtoWorld.Inverse() * result.worldPointB;

    result.worldPos = f.a.position;
    // From "Game Physics", pg. 531
    //ecb()
    //float test = glm::length(result.worldPointB - result.worldPointA);
    return result;
}