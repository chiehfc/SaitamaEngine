#pragma once
#include <DirectXMath.h>

using namespace DirectX::SimpleMath;

class Transform {

private:

    // -------------------- Attributes -------------------- //

    /// Position
    Vector3 mPosition;

    /// Orientation
    Quaternion mOrientation;

public:

    // -------------------- Methods -------------------- //

    /// Constructor
    Transform();

    /// Constructor
    Transform(const Vector3& position, const Matrix& orientation);

    /// Constructor
    Transform(const Vector3& position, const Quaternion& orientation);

    /// Destructor
    ~Transform() = default;

    /// Copy-constructor
    Transform(const Transform& transform);

    /// Return the origin of the transform
    const Vector3& getPosition() const;

    /// Set the origin of the transform
    void setPosition(const Vector3& position);

    /// Return the orientation quaternion
    const Quaternion& getOrientation() const;

    /// Set the rotation quaternion
    void setOrientation(const Quaternion& orientation);

    /// Set the transform to the identity transform
    void setToIdentity();

    /// Return the inverse of the transform
    Transform getInverse() const;

    /// Return an interpolated transform
    static Transform interpolateTransforms(const Transform& oldTransform,
        const Transform& newTransform,
        float interpolationFactor);

    /// Return the identity transform
    static Transform identity();

    /// Return the transformed vector
    Vector3 operator*(const Vector3& vector) const;

    /// Operator of multiplication of a transform with another one
    Transform operator*(const Transform& transform2) const;

    /// Return true if the two transforms are equal
    bool operator==(const Transform& transform2) const;

    /// Return true if the two transforms are different
    bool operator!=(const Transform& transform2) const;

    /// Assignment operator
    Transform& operator=(const Transform& transform);
};

// Constructor
inline Transform::Transform() : mPosition(Vector3(0.0, 0.0, 0.0)), mOrientation(Quaternion::Identity) {

}

// Constructor
inline Transform::Transform(const Vector3& position, const Matrix& orientation)
    : mPosition(position), mOrientation(Quaternion::CreateFromRotationMatrix(orientation)) {

}

// Constructor
inline Transform::Transform(const Vector3& position, const Quaternion& orientation)
    : mPosition(position), mOrientation(orientation) {

}

// Copy-constructor
inline Transform::Transform(const Transform& transform)
    : mPosition(transform.mPosition), mOrientation(transform.mOrientation) {

}

// Return the position of the transform
inline const Vector3& Transform::getPosition() const {
    return mPosition;
}

// Set the origin of the transform
inline void Transform::setPosition(const Vector3& position) {
    mPosition = position;
}

// Return the rotation matrix
inline const Quaternion& Transform::getOrientation() const {
    return mOrientation;
}

// Set the rotation matrix of the transform
inline void Transform::setOrientation(const Quaternion& orientation) {
    mOrientation = orientation;
}

// Set the transform to the identity transform
inline void Transform::setToIdentity() {
    mPosition = Vector3(0.0, 0.0, 0.0);
    mOrientation = Quaternion::Identity;
}

// Return the inverse of the transform
inline Transform Transform::getInverse() const {
    Quaternion invQuaternion;
    mOrientation.Inverse(invQuaternion);

    const float prodX = invQuaternion.w * -mPosition.x + invQuaternion.y * -mPosition.z - invQuaternion.z * -mPosition.y;
    const float prodY = invQuaternion.w * -mPosition.y + invQuaternion.z * -mPosition.x - invQuaternion.x * -mPosition.z;
    const float prodZ = invQuaternion.w * -mPosition.z + invQuaternion.x * -mPosition.y - invQuaternion.y * -mPosition.x;
    const float prodW = -invQuaternion.x * -mPosition.x - invQuaternion.y * -mPosition.y - invQuaternion.z * -mPosition.z;
    Vector3 pos = Vector3(invQuaternion.w * prodX - prodY * invQuaternion.z + prodZ * invQuaternion.y - prodW * invQuaternion.x,
        invQuaternion.w * prodY - prodZ * invQuaternion.x + prodX * invQuaternion.z - prodW * invQuaternion.y,
        invQuaternion.w * prodZ - prodX * invQuaternion.y + prodY * invQuaternion.x - prodW * invQuaternion.z);



    return Transform(pos, invQuaternion);
}

// Return an interpolated transform
inline Transform Transform::interpolateTransforms(const Transform& oldTransform,
    const Transform& newTransform,
    float interpolationFactor) {

    Vector3 interPosition = oldTransform.mPosition * (float(1.0) - interpolationFactor) +
        newTransform.mPosition * interpolationFactor;

    Quaternion interOrientation = Quaternion::Slerp(oldTransform.mOrientation,
        newTransform.mOrientation,
        interpolationFactor);

    return Transform(interPosition, interOrientation);
}

// Return the identity transform
inline Transform Transform::identity() {
    return Transform(Vector3(0, 0, 0), Quaternion::Identity);
}

// Return the transformed vector
inline Vector3 Transform::operator*(const Vector3& point) const {

    const float prodX = mOrientation.w * point.x + mOrientation.y * point.z - mOrientation.z * point.y;
    const float prodY = mOrientation.w * point.y + mOrientation.z * point.x - mOrientation.x * point.z;
    const float prodZ = mOrientation.w * point.z + mOrientation.x * point.y - mOrientation.y * point.x;
    const float prodW = -mOrientation.x * point.x - mOrientation.y * point.y - mOrientation.z * point.z;
    return Vector3(mOrientation.w * prodX - prodY * mOrientation.z + prodZ * mOrientation.y - prodW * mOrientation.x,
        mOrientation.w * prodY - prodZ * mOrientation.x + prodX * mOrientation.z - prodW * mOrientation.y,
        mOrientation.w * prodZ - prodX * mOrientation.y + prodY * mOrientation.x - prodW * mOrientation.z)
        + mPosition;
}

// Operator of multiplication of a transform with another one
inline Transform Transform::operator*(const Transform& transform2) const {

    // The following code is equivalent to this
    //return Transform(mPosition + mOrientation * transform2.mPosition,
    //                 mOrientation * transform2.mOrientation);

    const float prodX = mOrientation.w * transform2.mPosition.x + mOrientation.y * transform2.mPosition.z
        - mOrientation.z * transform2.mPosition.y;
    const float prodY = mOrientation.w * transform2.mPosition.y + mOrientation.z * transform2.mPosition.x
        - mOrientation.x * transform2.mPosition.z;
    const float prodZ = mOrientation.w * transform2.mPosition.z + mOrientation.x * transform2.mPosition.y
        - mOrientation.y * transform2.mPosition.x;
    const float prodW = -mOrientation.x * transform2.mPosition.x - mOrientation.y * transform2.mPosition.y
        - mOrientation.z * transform2.mPosition.z;

    return Transform(Vector3(mPosition.x + mOrientation.w * prodX - prodY * mOrientation.z + prodZ * mOrientation.y - prodW * mOrientation.x,
        mPosition.y + mOrientation.w * prodY - prodZ * mOrientation.x + prodX * mOrientation.z - prodW * mOrientation.y,
        mPosition.z + mOrientation.w * prodZ - prodX * mOrientation.y + prodY * mOrientation.x - prodW * mOrientation.z),
        Quaternion(mOrientation.w * transform2.mOrientation.x + transform2.mOrientation.w * mOrientation.x
            + mOrientation.y * transform2.mOrientation.z - mOrientation.z * transform2.mOrientation.y,
            mOrientation.w * transform2.mOrientation.y + transform2.mOrientation.w * mOrientation.y
            + mOrientation.z * transform2.mOrientation.x - mOrientation.x * transform2.mOrientation.z,
            mOrientation.w * transform2.mOrientation.z + transform2.mOrientation.w * mOrientation.z
            + mOrientation.x * transform2.mOrientation.y - mOrientation.y * transform2.mOrientation.x,
            mOrientation.w * transform2.mOrientation.w - mOrientation.x * transform2.mOrientation.x
            - mOrientation.y * transform2.mOrientation.y - mOrientation.z * transform2.mOrientation.z));
}

// Return true if the two transforms are equal
inline bool Transform::operator==(const Transform& transform2) const {
    return (mPosition == transform2.mPosition) && (mOrientation == transform2.mOrientation);
}

// Return true if the two transforms are different
inline bool Transform::operator!=(const Transform& transform2) const {
    return !(*this == transform2);
}

// Assignment operator
inline Transform& Transform::operator=(const Transform& transform) {
    if (&transform != this) {
        mPosition = transform.mPosition;
        mOrientation = transform.mOrientation;
    }
    return *this;
}