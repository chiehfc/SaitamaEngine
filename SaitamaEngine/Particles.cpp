#include "pch.h"
#include "Particles.h"

void Particles::integrate(double duration)
{
}

void Particles::setMass(const double mass)
{
}

double Particles::getMass() const
{
    return 0.0;
}

void Particles::setInverseMass(const double inverseMass)
{
}

double Particles::getInverseMass() const
{
    return 0.0;
}

bool Particles::hasFiniteMass() const
{
    return false;
}

void Particles::setDamping(const double damping)
{
}

double Particles::getDamping() const
{
    return 0.0;
}

void Particles::setPosition(const Vector3 & position)
{
}

void Particles::setPosition(const double x, const double y, const double z)
{
}

void Particles::getPosition(Vector3 * position) const
{
}

Vector3 Particles::getPosition() const
{
    return Vector3();
}

void Particles::setVelocity(const Vector3 & velocity)
{
}

void Particles::setVelocity(const double x, const double y, const double z)
{
}

void Particles::getVelocity(Vector3 * velocity) const
{
}

Vector3 Particles::getVelocity() const
{
    return Vector3();
}

void Particles::setAcceleration(const Vector3 & acceleration)
{
}

void Particles::setAcceleration(const double x, const double y, const double z)
{
}

void Particles::getAcceleration(Vector3 * acceleration) const
{
}

Vector3 Particles::getAcceleration() const
{
    return Vector3();
}

void Particles::clearAccumulator()
{
}

void Particles::addForce(const Vector3 & force)
{
}
