#pragma once
#include "Saitama.h"
#include "Particles.h"
#include "RigidBody.h"
#include "CollisionNarrow.h"
#include "CollisionPrimitive.h"
#include "Manifold.h"
#include "ConstraintSolverSeqImpulse.h"

class PhysicsSystem
{
public:
    PhysicsSystem();
    ~PhysicsSystem();

    void Initialize();

    void OnUpdate(double delta);

    void StepSimulation(float timeStep, int maxSubStep = 1, float fixedTimeStep = 1.0f / 60.0f);

    void SingleSimulationStep(float fixedTimeStep);

    void VApplyForce(const Vector3 &dir, float newtons, GameObjectId gameObjectId);


    void VSyncVisibleScene();

    // Temporarily for particle physics
    void VAddParticle(StrongGameObjectPtr pGameObject);
    void VAddRigidBody(StrongGameObjectPtr pGameObject);
    void VRemoveGameObject(GameObjectId id);

    void VAddCollider(StrongGameObjectPtr pGameObject);


    Vector3 VGetVelocity(GameObjectId gameObjectId);
    void VSetVelocity(GameObjectId gameObjectId, const Vector3& vel);
    void VTranslate(GameObjectId gameObjectId, const Vector3& vec);

    void VSetTransform(const GameObjectId id, const Vector3& mat);

    Matrix VGetTransform(const GameObjectId id);


    void CollisionDetection(float fixedTimeStep);

    void CollisionMovementUpdate(float fixedTimeStep);

private:
    typedef std::map<GameObjectId, Particles*> GameObjectIDToParticleMap;
    typedef std::map<GameObjectId, RigidBody*> GameObjectIDToRigidBodyMap;
    GameObjectIDToParticleMap m_gameObjectIdToParticle;
    GameObjectIDToRigidBodyMap m_gameObjectIdToRigidBody;
    Particles *FindParticle(GameObjectId id) const;
    RigidBody *FindRigidBody(GameObjectId id) const;
    std::vector<Particles *> m_particles;
    std::vector<RigidBody *> m_rigidBodies;

    PhysicsDef::ColliderPairList m_colliderPairList;

    //PersistentManifold m_persistentManifold;
    // Manifold map data structure
    typedef std::map<ManifoldKey, Manifold> ManifoldMap;
    typedef ManifoldMap::iterator ManifoldMapIter;
    ManifoldMap m_manifolds;
    ConstraintSolverSeqImpulse m_constraintSolver;

    GJK gjk;

    float m_fixedTimeStep = 0;
    float m_localTime = 0;
};

