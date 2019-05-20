#pragma once

using namespace DirectX::SimpleMath;

class Particles
{
public:

    void integrate(double duration);

    void setMass(const double mass);

    double getMass() const;

    void setInverseMass(const double inverseMass);

    double getInverseMass() const;

    bool hasFiniteMass() const;

    void setDamping(const double damping);

    double getDamping() const;

    void setPosition(const Vector3 &position);

    void setPosition(const double x, const double y, const double z);

    void getPosition(Vector3 *position) const;

    Vector3 getPosition() const;

    void setVelocity(const Vector3 &velocity);

    void setVelocity(const double x, const double y, const double z);

    void getVelocity(Vector3 *velocity) const;

    Vector3 getVelocity() const;

    void setAcceleration(const Vector3 &acceleration);

    void setAcceleration(const double x, const double y, const double z);

    void getAcceleration(Vector3 *acceleration) const;

    Vector3 getAcceleration() const;

    void clearAccumulator();

    void addForce(const Vector3 &force);

    Matrix getTransform() const { return m_transform; }

    void setTransform(Matrix &transform) { m_transform = transform; }

protected:
    
    double m_inverseMass;

    double m_damping;

    Vector3 m_position;

    Vector3 m_velocity;

    Vector3 m_forceAccum;

    Vector3 m_acceleration;

    Matrix m_transform;
};

