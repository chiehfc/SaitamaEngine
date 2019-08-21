#include "pch.h"
#include "Manifold.h"


using namespace PhysicsDef;


Manifold::Manifold(RigidBody *a, RigidBody *b)
{
    if (a < b)
    {
        m_bodyA = a;
        m_bodyB = b;
    } else
    {
        m_bodyA = b;
        m_bodyB = a;
    }

}

void Manifold::update(ContactData *newContacts, int newContactCount)
{
    std::vector<ContactData> mergedContacts;

    ContactData *newC, *oldC, *c;
    //Merge the old and new contacts
    //Based on Allen Chou's Game Physics series
    if (m_contacts.size() == 0)
    {
        for (int i = 0; i < newContactCount; ++i)
        {
            m_contacts.push_back(newContacts[i]);
        }

        return;
    }
    //Merge points, except those too close to an old point
    Vector3 rA, rB;
    for (int i = 0; i < newContactCount; ++i)
    {
        for (int j = 0; j < m_contacts.size(); ++j)
        {
            rA = newContacts[i].localPositionA - m_contacts[j].localPositionA;
            rB = newContacts[i].localPositionB - m_contacts[j].localPositionB;

            if (rA.Dot(rA) > 0.001 && rB.Dot(rB) > 0.001)
            {
                m_contacts.push_back(newContacts[i]);
            }

        }

    }

    //Find deepest point
    //TODO: change to pointer
    //TODO: get rid of the bool
    bool found = false;
    ContactData deepest;
    float currMax = -FLT_MAX;
    for (int i = 0; i < m_contacts.size(); ++i)
    {
        if (m_contacts[i].depth > currMax)
        {
            currMax = m_contacts[i].depth;
            deepest = m_contacts[i];
            found = true;
        }
    }

    if (found)
        mergedContacts.push_back(deepest);

    //Find second point, furthest from the deepest point
    ContactData point1;
    Vector3 diff;
    float distance;
    currMax = -FLT_MAX;
    found = false;
    for (int i = 0; i < m_contacts.size(); ++i)
    {
        diff = m_contacts[i].localPositionA - deepest.localPositionA;
        distance = diff.Dot(diff);
        if (distance > currMax)
        {
            currMax = distance;
            point1 = m_contacts[i];
            found = true;
        }
    }

    if (found)
        mergedContacts.push_back(point1);

    //Find third point, furthest from the line between deepest and point1
    ContactData point2;
    currMax = -FLT_MAX;
    found = false;
    for (int i = 0; i < m_contacts.size(); ++i)
    {
        distance = DistanceToLineSq(deepest.localPositionA, point1.localPositionA, m_contacts[i].localPositionA);
        if (distance > currMax)
        {
            currMax = distance;
            point2 = m_contacts[i];
            found = true;
        }
    }

    if (found)
        mergedContacts.push_back(point2);

    //Find fourth point, furthest from triangle formed by deepest, point1 and point2
    ContactData point3;
    currMax = -FLT_MAX;
    found = false;
    for (int i = 0; i < m_contacts.size(); ++i)
    {
        distance = DistanceToTriangleSq(deepest.localPositionA, point1.localPositionA, point2.localPositionA, m_contacts[i].localPositionA);
        if (distance > currMax)
        {
            currMax = distance;
            point3 = m_contacts[i];
            found = true;
        }
    }

    if (found)
        mergedContacts.push_back(point3);

    m_contacts = mergedContacts;
}

void Manifold::reduce()
{

}

//
//void PersistentManifold::addContactManifold(Manifold *manifold)
//{
//    m_manifolds.insert(manifold);
//}
//
//void PersistentManifold::clearAllManifolds()
//{
//    m_manifolds.clear();
//}