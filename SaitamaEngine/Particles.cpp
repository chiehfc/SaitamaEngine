#include "pch.h"
#include "Particles.h"

using namespace DirectX::SimpleMath;

void Particles::integrate(double duration)
{
    if (m_inverseMass <= 0.0f) return;

    m_position += m_velocity * duration + m_acceleration * duration * duration * 0.5f;

    m_velocity += m_acceleration * duration;
    
    m_velocity *= pow(m_damping, duration);

    clearAccumulator();
}

void Particles::setMass(const double mass)
{
    assert(mass != 0);
    m_inverseMass = ((double)1.0 / mass);
}

double Particles::getMass() const
{
    if (m_inverseMass == 0) 
    {
        return DBL_MAX;
    } 
    else
    {
        return ((double)1.0 / m_inverseMass);
    }
}

void Particles::setInverseMass(const double inverseMass)
{
    m_inverseMass = inverseMass;
}

double Particles::getInverseMass() const
{
    return m_inverseMass;
}

bool Particles::hasFiniteMass() const
{
    return m_inverseMass >= 0.0f;
}

void Particles::setDamping(const double damping)
{
    m_damping = damping;
}

double Particles::getDamping() const
{
    return m_damping;
}

void Particles::setPosition(const Vector3 &position)
{
    m_position = position;
}

void Particles::setPosition(const double x, const double y, const double z)
{
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
}

void Particles::getPosition(Vector3 *position) const
{
    *position = m_position;
}

Vector3 Particles::getPosition() const
{
    return m_position;
}

void Particles::setVelocity(const Vector3 &velocity)
{
    m_velocity = velocity;
}

void Particles::setVelocity(const double x, const double y, const double z)
{
    m_velocity.x = x;
    m_velocity.y = y;
    m_velocity.z = z;
}

void Particles::getVelocity(Vector3 *velocity) const
{
    *velocity = m_velocity;
}

Vector3 Particles::getVelocity() const
{
    return m_velocity;
}

void Particles::setAcceleration(const Vector3 &acceleration)
{
    m_acceleration = acceleration;
}

void Particles::setAcceleration(const double x, const double y, const double z)
{
    m_acceleration.x = x;
    m_acceleration.y = y;
    m_acceleration.z = z;
}

void Particles::getAcceleration(Vector3 *acceleration) const
{
    *acceleration = m_acceleration;
}

Vector3 Particles::getAcceleration() const
{
    return m_acceleration;
}

void Particles::clearAccumulator()
{
    m_forceAccum = Vector3::Zero;
}

void Particles::addForce(const Vector3 &force)
{
    m_forceAccum += force;
}
