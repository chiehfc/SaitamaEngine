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
}