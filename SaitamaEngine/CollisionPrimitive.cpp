#include "pch.h"
#include "CollisionPrimitive.h"


PhysicsDef::CollisionType CollisionPrimitive::getType() const
{
    return m_type;
}


CollisionBox::CollisionBox(const Vector3 &extents)
{
    m_extents = extents;
    m_type = PhysicsDef::CollisionType::Box;
}

Vector3 CollisionBox::getFarthestPointInDirection(Vector3 direction)
{
    auto dir = Vector3::Transform(direction, matRS_inverse); //find support in model space

    Vector3 result;
    result.x = (dir.x > 0) ? max.x : min.x;
    result.y = (dir.y > 0) ? max.y : min.y;
    result.z = (dir.z > 0) ? max.z : min.z;

    return Vector3::Transform(result, matRS_inverse) + pos; //convert support to world space
}

Matrix CollisionBox::getTensor(float mass)
{
    //TODO: FIX FOR SCALING
    Matrix result;

    result._11 = mass / 12 * (m_extents.y * m_extents.y + m_extents.z * m_extents.z);
    result._22 = mass / 12 * (m_extents.x * m_extents.x + m_extents.z * m_extents.z);
    result._33 = mass / 12 * (m_extents.y * m_extents.y + m_extents.x * m_extents.x);

    return result;
}

Vector3 CollisionBox::getSupportPoint(const Vector3 &dir) const
{
    Vector3 result;
    float dot, max = FLT_MIN;

    Vector3 extents = m_extents * 0.5f;
    std::vector<Vector3> vertices;
    vertices.push_back(Vector3(extents.x, -extents.y, -extents.z));
    vertices.push_back(Vector3(extents.x, extents.y, -extents.z));
    vertices.push_back(Vector3(extents.x, -extents.y, extents.z));
    vertices.push_back(Vector3(extents.x, extents.y, extents.z));
    vertices.push_back(Vector3(-extents.x, -extents.y, -extents.z));
    vertices.push_back(Vector3(-extents.x, extents.y, -extents.z));
    vertices.push_back(Vector3(-extents.x, -extents.y, extents.z));
    vertices.push_back(Vector3(-extents.x, extents.y, extents.z));

    for (int i = 0; i < vertices.size(); ++i)
    {
        dot = dir.Dot(vertices[i]);
        if (dot > max)
        {
            max = dot;
            result = vertices[i];
        }
    }

    // result is in local space
    return result;
}