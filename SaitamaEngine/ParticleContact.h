#pragma once
#include "Particles.h"
class ParticleContact
{
public:
    /**
     * Holds the particles that are involved in the contact. The
     * second of these can be NULL, for contacts with the scenery.
     */
    Particles* particle[2];

    /**
     * Holds the normal restitution coefficient at the contact.
     */
    double restitution;

    /**
     * Holds the direction of the contact in world coordinates.
     */
    Vector3 contactNormal;

    /**
     * Holds the depth of penetration at the contact.
     negative penetration means no interpenetration
     zero penetration means merely touching
     */
    double penetration;

    /**
     * Holds the amount each particle is moved by during interpenetration
     * resolution.
     */
    Vector3 particleMovement[2];

protected:
    /**
     * Resolves this contact, for both velocity and interpenetration.
     */
    void resolve(double duration);

    /**
     * Calculates the separating velocity at this contact.
     */
    double calculateSeparatingVelocity() const;

private:
    /**
     * Handles the impulse calculations for this collision.
     */
    void resolveVelocity(double duration);

    /**
     * Handles the interpenetration resolution for this contact.
     */
    void resolveInterpenetration(double duration);
};

