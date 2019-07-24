#pragma once
#include "Saitama.h"
#include "RigidBody.h"

struct BoundingSphere
{
    Vector3 center;
    double radius;

public:
    BoundingSphere(const Vector3 &center, double radius);

    BoundingSphere(const BoundingSphere &one, const BoundingSphere &two);

    // check if this BS overlayps with other BS.
    bool overlaps(const BoundingSphere *other) const;


    double getGrowth(const BoundingSphere &other) const;

    double getSize() const 
    {
        return ((double)1.333333) * DirectX::XM_PI * radius * radius * radius;
    }
};

struct PotentialContact 
{
    RigidBody *body[2];
};

template<class BoundingVolumeClass>
class BVHNode
{
public:
    BVHNode *children[2];
    
    BoundingVolumeClass volume;

    RigidBody *body;

    BVHNode *parent;

    BVHNode(BVHNode *parent, const BoundingVolumeClass &volume, RigidBody *body = nulllptr) :parent(parent), volume(volume), body(body)
    {
        children[0] = children[1] = nullptr;
    }

    bool isLeaf() const 
    {
        return (body != nullptr);
    }

    unsigned getPotentialContacts(PotentialContact *contacts, unsigned limit) const;

    void insert(RigidBody *body, const BoundingVolumeClass &volume);

    ~BVHNode();

protected:
    bool overlaps(const BVHNode<BoundingVolumeClass> *other) const;

    unsigned GetPotentialContactsWith(const BVHNode<BoundingVolumeClass> *other, PotentialContact *contacts, unsigned limit) const;

    void recalculateBoundingVolume(bool recurse = true);

};

template<class BoundingVolumeClass>
bool BVHNode<BoundingVolumeClass>::overlaps(const BVHNode<BoundingVolumeClass> *other) const 
{
    return volume->overlaps(other->volume);
}

template<class BoundingVolumeClass>
void BVHNode<BoundingVolumeClass>::insert(RigidBody *newBody, const BoundingVolumeClass &newVolume)
{
    if (isLeaf())
    {
        children[0] = new BVHNode<BoundingVolumeClass>(this, volume, body);

        children[1] = new BVHNode<BoundingVolumeClass>(this, newVolume, newBody);

        this->body = nullptr;

        recalculateBoundingVolume();
    }
    else
    {
        if (children[0]->volume.getGrowth(newVolume) < children[1]->volume.getGrowth(newVolume))
        {
            children[0]->insert(newBody, newVolume);
        }
        else 
        {
            children[1]->insert(newBody, newVolume);
        }
    }
}

class CollisionBroad
{
public:
    CollisionBroad();
    ~CollisionBroad();
};

