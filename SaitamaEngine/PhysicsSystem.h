#pragma once
#include "Saitama.h"
#include "Particles.h"

class PhysicsSystem
{
public:
    PhysicsSystem();
    ~PhysicsSystem();

    void Initialize();

    void OnUpdate(double delta);


    void VApplyForce(const Vector3 &dir, float newtons, GameObjectId gameObjectId);

    // Temporarily for particle physics
    void VAddParticle(WeakGameObjectPtr pGameObject);
    void VRemoveGameObject(GameObjectId id);


    Vector3 VGetVelocity(GameObjectId gameObjectId);
    void VSetVelocity(GameObjectId gameObjectId, const Vector3& vel);
    void VTranslate(GameObjectId gameObjectId, const Vector3& vec);

    void VSetTransform(const GameObjectId id, const Vector3& mat);

    Matrix VGetTransform(const GameObjectId id);


private:
    typedef std::map<GameObjectId, Particles*> GameObjectIDToParticleMap;
    GameObjectIDToParticleMap m_gameObjectIdToParticle;


};

