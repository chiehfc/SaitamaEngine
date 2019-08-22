#include "pch.h"
#include "ConstraintSolverSeqImpulse.h"

using namespace PhysicsDef;

void ConstraintSolverSeqImpulse::SolveConstraints2(std::vector<Manifold>& manifolds, float dt)
{

    PreStep(manifolds, dt);

    for (int j = 0; j < 10; ++j)
    {
        for (int i = 0; i < manifolds.size(); ++i)
        {
            for (int k = 0; k < manifolds[i].m_contacts.size(); ++k)
            {
                SolveContact(manifolds[i].m_bodyA, manifolds[i].m_bodyB, manifolds[i].m_contacts[k], dt);
            }
        }
    }
}

float CalcJV(const Vector3 &normal,
    const Vector3 &r1, const Vector3 &lVel1, const Vector3 &aVel1,
    const Vector3 &r2, const Vector3 &lVel2, const Vector3 &aVel2
)
{
    float JV;
    float lComp1 = normal.Dot(lVel1);
    float lComp2 = normal.Dot(lVel2);
    float aComp1 = aVel1.Dot(normal.Cross(r1));
    float aComp2 = aVel2.Dot(normal.Cross(r2));

    JV = -lComp1 - aComp1 + lComp2 + aComp2;

    return JV;

}
void ConstraintSolverSeqImpulse::PreStep(std::vector<Manifold>& manifolds, float dt)
{
    ContactData *contact;
    float invM1, invM2, totalInvMass, minRest, friction;
    Vector3 localANorm, localBNorm, localATangent, localBTangent, vel1, vel2, aVel1, aVel2, impulse, torque1, torque2;
    Matrix invTensor1, invTensor2;
    float JV;
    for (int i = 0; i < manifolds.size(); ++i)
    {
        invM1 = manifolds[i].m_bodyA->getInverseMass();
        invM2 = manifolds[i].m_bodyB->getInverseMass();
        invTensor1 = manifolds[i].m_bodyA->getInverseInertiaTensor();
        invTensor2 = manifolds[i].m_bodyB->getInverseInertiaTensor();
        vel1 = manifolds[i].m_bodyA->getLinearVelocity();
        vel2 = manifolds[i].m_bodyB->getLinearVelocity();
        aVel1 = manifolds[i].m_bodyA->getAngularVelocity();
        aVel2 = manifolds[i].m_bodyB->getAngularVelocity();
        minRest = std::min(manifolds[i].m_bodyA->getRestitution(), manifolds[i].m_bodyB->getRestitution());
        friction = manifolds[i].m_bodyA->getFriction() * manifolds[i].m_bodyB->getFriction();
        totalInvMass = invM1 + invM2;
        for (int k = 0; k < manifolds[i].m_contacts.size(); ++k)
        {
            contact = &manifolds[i].m_contacts[k];
            //Normal Mass
            localANorm = contact->localPositionA.Cross(contact->normal);
            localBNorm = contact->localPositionB.Cross(contact->normal);

            Vector3 localANormMulInvTensor1 = DirectX::XMVector3Transform(localANorm, invTensor1);
            Vector3 localBNormMulInvTensor2 = DirectX::XMVector3Transform(localBNorm, invTensor2);

            contact->massNormal = 1.f / (totalInvMass + localANormMulInvTensor1.Dot(localANorm) +
                localBNormMulInvTensor2.Dot(localBNorm));

            ////TODO: TANGENT
            //Based on Errin Catto's "Computing a Basis" article
            if (std::abs(contact->normal.x) >= 0.57735f)
                contact->tangent1 = Vector3(-contact->normal.y, contact->normal.x, 0.0f);
            else
                contact->tangent1 = Vector3(0.0f, contact->normal.z, -contact->normal.y);
            contact->tangent1.Normalize(contact->tangent1);
            contact->tangent2 = contact->normal.Cross(contact->tangent1);

            //Mass tangent 1
            localATangent = contact->localPositionA.Cross(contact->tangent1);
            localBTangent = contact->localPositionB.Cross(contact->tangent1);

            Vector3 localATangentMulInvTensor1 = DirectX::XMVector3Transform(localATangent, invTensor1);
            Vector3 localBTangentMulInvTensor2 = DirectX::XMVector3Transform(localBTangent, invTensor2);

            contact->massTangent1 = 1.f / (totalInvMass + localATangentMulInvTensor1.Dot(localATangent) +
                localBTangentMulInvTensor2.Dot(localBTangent));

            //Mass tangent 2
            localATangent = contact->localPositionA.Cross(contact->tangent2);
            localBTangent = contact->localPositionB.Cross(contact->tangent2);

            localATangentMulInvTensor1 = DirectX::XMVector3Transform(localATangent, invTensor1);
            localBTangentMulInvTensor2 = DirectX::XMVector3Transform(localBTangent, invTensor2);;

            contact->massTangent2 = 1.f / (totalInvMass + localATangentMulInvTensor1.Dot(localATangent) +
                localBTangentMulInvTensor2.Dot(localBTangent));

            //Bias
            JV = CalcJV(contact->normal, contact->localPositionA, vel1, aVel1, contact->localPositionB, vel2, aVel2);
            contact->bias = -0.2f / dt * std::max(0.0f, contact->depth - 0.1f) + minRest * JV;
            contact->friction = friction;

            if (manifolds[i].m_isPersistent)
            {

                impulse = Vector3(contact->normal.x * contact->prevNormalImp,
                    contact->normal.y * contact->prevNormalImp,
                    contact->normal.z * contact->prevNormalImp);
                torque1 = contact->localPositionA.Cross(impulse);
                torque2 = contact->localPositionB.Cross(impulse);
                manifolds[i].m_bodyA->applyImpulse(-impulse);
                manifolds[i].m_bodyB->applyImpulse(impulse);
                manifolds[i].m_bodyA->applyTorqueImpulse(torque1);
                manifolds[i].m_bodyB->applyTorqueImpulse(-torque2);

                impulse = Vector3(contact->tangent1.x * contact->prevTangImp1,
                    contact->tangent1.y * contact->prevTangImp1,
                    contact->tangent1.z * contact->prevTangImp1);
                torque1 = contact->localPositionA.Cross(impulse);
                torque2 = contact->localPositionB.Cross(impulse);
                manifolds[i].m_bodyA->applyImpulse(-impulse);
                manifolds[i].m_bodyB->applyImpulse(impulse);
                manifolds[i].m_bodyA->applyTorqueImpulse(torque1);
                manifolds[i].m_bodyB->applyTorqueImpulse(-torque2);

                impulse = Vector3(contact->tangent2.x * contact->prevTangImp2,
                    contact->tangent2.y * contact->prevTangImp2,
                    contact->tangent2.z * contact->prevTangImp2);
                torque1 = contact->localPositionA.Cross(impulse);
                torque2 = contact->localPositionB.Cross(impulse);
                manifolds[i].m_bodyA->applyImpulse(-impulse);
                manifolds[i].m_bodyB->applyImpulse(impulse);
                manifolds[i].m_bodyA->applyTorqueImpulse(torque1);
                manifolds[i].m_bodyB->applyTorqueImpulse(-torque2);
            }
        }
    }
}

void ConstraintSolverSeqImpulse::SolveContact(RigidBody *body1, RigidBody *body2, ContactData &contact, float dt)
{

    //TODO: FIND ERROR
    //glm::vec3 normal = contact.normal;

    Vector3 dv, impulse, impulseTangent, tangent, vel1, vel2, aVel1, aVel2;
    //body1->SetLinearVelocity(glm::vec3(0.f));
    //body2->SetLinearVelocity(glm::vec3(0.f));

    float massNormal, massTangent, invM1, invM2, f;
    Vector3 localANorm, localBNorm, localATang, localBTang;
    //glm::mat4 interpolationTrans1;
    //glm::mat4 interpolationTrans2;

    float friction, invDt = 1.f / dt;
    float JV;
    float lambda, oldLambda;

    Vector3 invMJ;
    Vector3 impulse1, impulse2, torque1, torque2;

    invM1 = body1->getInverseMass();
    invM2 = body2->getInverseMass();
    //localANorm = glm::cross(contact.localPointA, normal);
    //localBNorm = glm::cross(contact.localPointB, normal);

    vel1 = body1->getLinearVelocity();
    vel2 = body2->getLinearVelocity();
    //relativeVel = vel2 - vel1;
    aVel1 = body1->getAngularVelocity();
    aVel2 = body2->getAngularVelocity();
    localANorm = contact.localPositionA.Cross(aVel1);
    localBNorm = contact.localPositionB.Cross(aVel2);
    JV = CalcJV(contact.normal, contact.localPositionA, vel1, aVel1, contact.localPositionB, vel2, aVel2);

    lambda = -(JV + contact.bias) * contact.massNormal;

    {
        oldLambda = contact.prevNormalImp;
        contact.prevNormalImp = std::max(oldLambda + lambda, 0.f);
        lambda = contact.prevNormalImp - oldLambda;
    }


    impulse = Vector3(contact.normal.x * lambda,
        contact.normal.y * lambda,
        contact.normal.z * lambda);
    torque1 = contact.localPositionA.Cross(impulse);
    torque2 = contact.localPositionB.Cross(impulse);
    body1->applyImpulse(-impulse);
    body2->applyImpulse(impulse);
    body1->applyTorqueImpulse(torque1);
    body2->applyTorqueImpulse(-torque2);

    float coeff = contact.prevNormalImp * contact.friction;
    vel1 = body1->getLinearVelocity();
    vel2 = body2->getLinearVelocity();
    //relativeVel = vel2 - vel1;
    aVel1 = body1->getAngularVelocity();
    aVel2 = body2->getAngularVelocity();
    //Apply friction for tangent1
    JV = -vel1.Dot(contact.tangent1) - localANorm.Dot(contact.tangent1)
        + vel2.Dot(contact.tangent1) + localBNorm.Dot(contact.tangent1);
    lambda = -JV * contact.massTangent1;
    {
        oldLambda = contact.prevTangImp1;
        contact.prevTangImp1 = Clamp(oldLambda + lambda, -coeff, coeff);
        lambda = contact.prevTangImp1 - oldLambda;
    }
    impulse = Vector3(contact.tangent1.x * lambda,
        contact.tangent1.y * lambda,
        contact.tangent1.z * lambda);
    torque1 = contact.localPositionA.Cross(impulse);
    torque2 = contact.localPositionB.Cross(impulse);
    body1->applyImpulse(-impulse);
    body2->applyImpulse(impulse);
    body1->applyTorqueImpulse(torque1);
    body2->applyTorqueImpulse(-torque2);

    //Apply friction for tangent2
    //vel1 = body1->GetLinearVelocity();
    //vel2 = body2->GetLinearVelocity();
    ////relativeVel = vel2 - vel1;
    //aVel1 = body1->GetAngularVelocity();
    //aVel2 = body2->GetAngularVelocity();
    JV = -vel1.Dot(contact.tangent2) - localANorm.Dot(contact.tangent2)
        + vel2.Dot(contact.tangent2) + localBNorm.Dot(contact.tangent2);
    lambda = -JV * contact.massTangent2;
    {
        oldLambda = contact.prevTangImp2;
        contact.prevTangImp2 = Clamp(oldLambda + lambda, -coeff, coeff);
        lambda = contact.prevTangImp2 - oldLambda;
    }
    impulse = Vector3(contact.tangent2.x * lambda,
        contact.tangent2.y * lambda,
        contact.tangent2.z * lambda);
    torque1 = contact.localPositionA.Cross(impulse);
    torque2 = contact.localPositionB.Cross(impulse);
    body1->applyImpulse(-impulse);
    body2->applyImpulse(impulse);
    body1->applyTorqueImpulse(torque1);
    body2->applyTorqueImpulse(-torque2);
}