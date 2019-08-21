#pragma once
#include <DirectXMath.h>
#include "SimpleMath.h"
#include <list>
#include "Transform.h"

class CollisionPrimitive;
class RigidBody;

using namespace DirectX::SimpleMath;

namespace PhysicsDef
{
    enum CollisionType
    {
        Box = 1,
        Sphere = 2,
    };

    struct SupportPoint
    {
        Vector3 position;
        Vector3 originA;
        Vector3 originB;
        Vector3 direction;
    };

    struct ContactData
    {

        // contact point data
        Vector3 globalPositionA;
        Vector3 globalPositionB;
        Vector3 localPositionA;
        Vector3 localPositionB;

        // these 3 vectors form an orthonormal basis
        Vector3 normal; // points from colliderA to colliderB
        Vector3 tangent1, tangent2;

        // penetration depth
        float depth;

        // for clamping (more on this later)
        float normalImpulseSum;
        float tangentImpulseSum1;
        float tangentImpulseSum2;

        ContactData(void)
            : normalImpulseSum(0.0f)
            , tangentImpulseSum1(0.0f)
            , tangentImpulseSum2(0.0f)
        { }

        //Calculated in prestep
        float massNormal;
        float massTangent1;
        float massTangent2;
        float bias;
        float friction;
    };


    struct RigidBodyConstructionInfo
    {
        float mass;
        float linearDamping;
        float angularDamping;
        float friction;
        float rollingFriction;
        float resititution;

        Transform transform;
        Vector3 localInertia;

        bool enableGravity;

        CollisionPrimitive* collisionShape = 0;

        
    };

    typedef std::pair<RigidBody *, RigidBody *> CollisionPair;
    typedef std::list<CollisionPair> ColliderPairList;
    typedef std::pair<CollisionPair, ContactData> CollPairContactInfo;


    inline float DistanceToLineSq(const Vector3 &start, const Vector3 &end, const Vector3 &p)
    {
        Vector3 pStart = p - start;
        Vector3 line = end - start;
        Vector3 proj = line * pStart.Dot(line) / line.Dot(line);
        Vector3 projP = p - proj;

        return projP.Dot(projP);
    }


    inline float DistanceToTriangleSq(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &p)
    {
        Vector3 normal;
        (b - a).Cross(c - a).Normalize(normal);
        Vector3 pa = p - a;
        float dotp = pa.Dot(normal);
        //Point projected onto normal
        Vector3 projN = dotp * normal;
        Vector3 diff = p - projN;
        //Point projected onto triangle, relative to A
        Vector3 projA = pa - projN;

        return dotp * dotp;
    }
}