#pragma once
#include "Manifold.h"
#include "RigidBody.h"

class ConstraintSolverSeqImpulse
{
public:
    void SolveConstraints2(std::vector<Manifold>& info, float dt);

private:
    void PreStep(std::vector<Manifold>& info, float dt);
    void SolveContact(RigidBody *body1, RigidBody *body2, PhysicsDef::ContactData &info, float dt);
};

