#pragma once
#include "Saitama.h"
#include "RigidBody.h"
#include <list>




struct AABB
{
    Vector3 lowerBound;
    Vector3 upperBound;


};

typedef std::list<AABB *> AABBList;

struct Ray3
{
    Vector3 pos;
    Vector3 dir;
};

struct RayCastResult
{
    bool hit;
    RigidBody *collider;
    Vector3 position;
    Vector3 normal;
};


class Broadphase
{
public:
    virtual void Add(AABB *aabb) = 0;
    virtual void Update(void) = 0;
    virtual const ColliderPairList &ComputePairs(void) = 0;
    virtual RigidBody *Pick(const Vector3 &point) const = 0;
    typedef std::vector<RigidBody *> ColliderList;

    virtual void Query(const AABB &aabb, ColliderList &output) const = 0;
    virtual RayCastResult RayCast(const Ray3 &ray) const = 0;
};



struct BoundingSphere
{
    Vector3 center;
    double radius;

public:
    BoundingSphere(const Vector3 &center, double radius);

    // Creates a bounding sphere to enclose the two given bounding spheres
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
    {
    else
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

template<class BoundingVolumeClass>
BVHNode<BoundingVolumeClass>::~BVHNode()
{
    if (parent)
    {
        BVHNode<BoundingVolumeClass> *sibling;
        if (parent->children[0] == this)
        {
            sibling = parent->children[1];
        }
        else
        {
            sibling = parent->children[0];
        }

        parent->volume = sibling->volume;
        parent->body = sibling->body;
        parent->children[0] = sibling->children[0];
        parent->children[1] = sibling->children[1];

        sibling->parent = nullptr;
        sibling->body = nullptr;
        sibling->children[0] = nullptr;
        sibling->children[1] = nullptr;
        delete sibling;

        parent->recalculateBoundingVolume();
    }
    if (children[0])
    {
        children[0] = nullptr;
        delete children[0];
    }
    if (children[1])
    {
        children[1] = nullptr;
        delete children[1];
    }
}

template<class BoundingVolumeClass>
void BVHNode<BoundingVolumeClass>::recalculateBoundingVolume(bool recurse)
{
    if (isLeaf()) return;

    volume = BoundingVolumeClass(children[0]->volume, children[1]->volume);

    if (parent)
    {
        parent->recalculateBoundingVolume();
    }
}

template<class BoundingVolumeClass>
unsigned BVHNode<BoundingVolumeClass>::getPotentialContacts(PotentialContact *contacts, unsigned limit) const
{
    if (isLeaf() || limit == 0) return 0;
    return children[0]->getPotentialContactsWith(children[1], contacts, limit);
}

template<class BoundingVolumeClass>
unsigned BVHNode<BoundingVolumeClass>::GetPotentialContactsWith(const BVHNode<BoundingVolumeClass> *other, PotentialContact *contacts, unsigned limit) const
{
    if (!overlaps(other) || limit == 0) return 0;
    if (isLeaf() && other->isLeaf())
    {
        contacts->body[0] = body;
        contacts->body[1] = other->body;
        return 1;
    }

    if (other->isLeaf() || (!isLeaf() && volume->getSize() >= other->volume->getSize()))
    {
        unsigned count = children[0]->getPotentialContactsWith(other, contacts, limit);

        if (limit > count) 
        {
            return count + children[1]->getPotentialContactsWith(other, contacts + count, limit - count);
        }
        else
        {
            return count;
        }
    }
    else
    {
        unsigned count = getPotentialContactsWith(other->children[0], contacts, limit);
        if (limit > count)
        {
            return count + GetPotentialContactsWith(other->children[1], contacts + count, limit - count);
        }
        else
        {
            return count;
        }
    }
}