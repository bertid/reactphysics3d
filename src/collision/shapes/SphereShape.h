/********************************************************************************
* ReactPhysics3D physics library, http://code.google.com/p/reactphysics3d/      *
* Copyright (c) 2010-2013 Daniel Chappuis                                       *
*********************************************************************************
*                                                                               *
* This software is provided 'as-is', without any express or implied warranty.   *
* In no event will the authors be held liable for any damages arising from the  *
* use of this software.                                                         *
*                                                                               *
* Permission is granted to anyone to use this software for any purpose,         *
* including commercial applications, and to alter it and redistribute it        *
* freely, subject to the following restrictions:                                *
*                                                                               *
* 1. The origin of this software must not be misrepresented; you must not claim *
*    that you wrote the original software. If you use this software in a        *
*    product, an acknowledgment in the product documentation would be           *
*    appreciated but is not required.                                           *
*                                                                               *
* 2. Altered source versions must be plainly marked as such, and must not be    *
*    misrepresented as being the original software.                             *
*                                                                               *
* 3. This notice may not be removed or altered from any source distribution.    *
*                                                                               *
********************************************************************************/

#ifndef REACTPHYSICS3D_SPHERE_SHAPE_H
#define REACTPHYSICS3D_SPHERE_SHAPE_H

// Libraries
#include "CollisionShape.h"
#include "../../body/CollisionBody.h"
#include "../../mathematics/mathematics.h"

// ReactPhysics3D namespace
namespace reactphysics3d {

// Class SphereShape
/**
 * This class represents a sphere collision shape that is centered
 * at the origin and defined by its radius. This collision shape does not
 * have an explicit object margin distance. The margin is implicitly the
 * radius of the sphere. Therefore, no need to specify an object margin
 * for a sphere shape.
 */
class SphereShape : public CollisionShape {

    private :

        // -------------------- Attributes -------------------- //

        /// Radius of the sphere
        decimal mRadius;

        // -------------------- Methods -------------------- //

        /// Private copy-constructor
        SphereShape(const SphereShape& shape);

        /// Private assignment operator
        SphereShape& operator=(const SphereShape& shape);

        /// Return true if a point is inside the collision shape
        bool testPointInside(const Vector3& localPoint) const;

    public :

        // -------------------- Methods -------------------- //

        /// Constructor
        SphereShape(decimal radius);

        /// Destructor
        virtual ~SphereShape();

        /// Allocate and return a copy of the object
        virtual SphereShape* clone(void* allocatedMemory) const;

        /// Return the radius of the sphere
        decimal getRadius() const;

        /// Return the number of bytes used by the collision shape
        virtual size_t getSizeInBytes() const;

        /// Return a local support point in a given direction with the object margin
        virtual Vector3 getLocalSupportPointWithMargin(const Vector3& direction) const;

        /// Return a local support point in a given direction without the object margin
        virtual Vector3 getLocalSupportPointWithoutMargin(const Vector3& direction) const;

        /// Return the local bounds of the shape in x, y and z directions.
        virtual void getLocalBounds(Vector3& min, Vector3& max) const;

        /// Return the local inertia tensor of the collision shape
        virtual void computeLocalInertiaTensor(Matrix3x3& tensor, decimal mass) const;

        /// Update the AABB of a body using its collision shape
        virtual void computeAABB(AABB& aabb, const Transform& transform);

        /// Test equality between two sphere shapes
        virtual bool isEqualTo(const CollisionShape& otherCollisionShape) const;

        /// Create a proxy collision shape for the collision shape
        virtual ProxyShape* createProxyShape(MemoryAllocator& allocator, CollisionBody* body,
                                             const Transform& transform, decimal mass);

        /// Raycast method
        virtual bool raycast(const Ray& ray, decimal distance = RAYCAST_INFINITY_DISTANCE) const;

        /// Raycast method with feedback information
        virtual bool raycast(const Ray& ray, RaycastInfo& raycastInfo,
                             decimal distance = RAYCAST_INFINITY_DISTANCE) const;

        // -------------------- Friendship -------------------- //

        friend class ProxySphereShape;
};


// Class ProxySphereShape
/**
 * The proxy collision shape for a sphere shape.
 */
class ProxySphereShape : public ProxyShape {

    private:

        // -------------------- Attributes -------------------- //

        /// Pointer to the actual collision shape
        SphereShape* mCollisionShape;


        // -------------------- Methods -------------------- //

        /// Private copy-constructor
        ProxySphereShape(const ProxySphereShape& proxyShape);

        /// Private assignment operator
        ProxySphereShape& operator=(const ProxySphereShape& proxyShape);

        /// Return the non-const collision shape
        virtual CollisionShape* getInternalCollisionShape() const;

    public:

        // -------------------- Methods -------------------- //

        /// Constructor
        ProxySphereShape(SphereShape* shape, CollisionBody* body,
                         const Transform& transform, decimal mass);

        /// Destructor
        ~ProxySphereShape();

        /// Return the collision shape
        virtual const CollisionShape* getCollisionShape() const;

        /// Return the number of bytes used by the proxy collision shape
        virtual size_t getSizeInBytes() const;

        /// Return a local support point in a given direction with the object margin
        virtual Vector3 getLocalSupportPointWithMargin(const Vector3& direction);

        /// Return a local support point in a given direction without the object margin
        virtual Vector3 getLocalSupportPointWithoutMargin(const Vector3& direction);

        /// Return the current collision shape margin
        virtual decimal getMargin() const;

        /// Raycast method
        virtual bool raycast(const Ray& ray, decimal distance = RAYCAST_INFINITY_DISTANCE) const;

        /// Raycast method with feedback information
        virtual bool raycast(const Ray& ray, RaycastInfo& raycastInfo,
                             decimal distance = RAYCAST_INFINITY_DISTANCE) const;

        /// Return true if a point is inside the collision shape
        virtual bool testPointInside(const Vector3& worldPoint);
};

/// Allocate and return a copy of the object
inline SphereShape* SphereShape::clone(void* allocatedMemory) const {
    return new (allocatedMemory) SphereShape(*this);
}

// Get the radius of the sphere
inline decimal SphereShape::getRadius() const {
    return mRadius;
}

// Return the number of bytes used by the collision shape
inline size_t SphereShape::getSizeInBytes() const {
    return sizeof(SphereShape);
}

// Return a local support point in a given direction with the object margin
inline Vector3 SphereShape::getLocalSupportPointWithMargin(const Vector3& direction) const {

    // If the direction vector is not the zero vector
    if (direction.lengthSquare() >= MACHINE_EPSILON * MACHINE_EPSILON) {

        // Return the support point of the sphere in the given direction
        return mMargin * direction.getUnit();
    }

    // If the direction vector is the zero vector we return a point on the
    // boundary of the sphere
    return Vector3(0, mMargin, 0);
}

// Return a local support point in a given direction without the object margin
inline Vector3 SphereShape::getLocalSupportPointWithoutMargin(const Vector3& direction) const {

    // Return the center of the sphere (the radius is taken into account in the object margin)
    return Vector3(0.0, 0.0, 0.0);
}

// Return the local bounds of the shape in x, y and z directions.
// This method is used to compute the AABB of the box
inline void SphereShape::getLocalBounds(Vector3& min, Vector3& max) const {

    // Maximum bounds
    max.x = mRadius;
    max.y = mRadius;
    max.z = mRadius;

    // Minimum bounds
    min.x = -mRadius;
    min.y = min.x;
    min.z = min.x;
}

// Return the local inertia tensor of the sphere
inline void SphereShape::computeLocalInertiaTensor(Matrix3x3& tensor, decimal mass) const {
    decimal diag = decimal(0.4) * mass * mRadius * mRadius;
    tensor.setAllValues(diag, 0.0, 0.0,
                        0.0, diag, 0.0,
                        0.0, 0.0, diag);
}

// Update the AABB of a body using its collision shape
inline void SphereShape::computeAABB(AABB& aabb, const Transform& transform) {

    // Get the local extents in x,y and z direction
    Vector3 extents(mRadius, mRadius, mRadius);

    // Update the AABB with the new minimum and maximum coordinates
    aabb.setMin(transform.getPosition() - extents);
    aabb.setMax(transform.getPosition() + extents);
}

// Test equality between two sphere shapes
inline bool SphereShape::isEqualTo(const CollisionShape& otherCollisionShape) const {
    const SphereShape& otherShape = dynamic_cast<const SphereShape&>(otherCollisionShape);
    return (mRadius == otherShape.mRadius);
}

// Create a proxy collision shape for the collision shape
inline ProxyShape* SphereShape::createProxyShape(MemoryAllocator& allocator, CollisionBody* body,
                                                 const Transform& transform, decimal mass)  {
    return new (allocator.allocate(sizeof(ProxySphereShape))) ProxySphereShape(this, body,
                                                                               transform, mass);
}

// Return the non-const collision shape
inline CollisionShape* ProxySphereShape::getInternalCollisionShape() const {
    return mCollisionShape;
}

// Return the collision shape
inline const CollisionShape* ProxySphereShape::getCollisionShape() const {
    return mCollisionShape;
}

// Return the number of bytes used by the proxy collision shape
inline size_t ProxySphereShape::getSizeInBytes() const {
    return sizeof(ProxySphereShape);
}

// Return a local support point in a given direction with the object margin
inline Vector3 ProxySphereShape::getLocalSupportPointWithMargin(const Vector3& direction) {
    return mCollisionShape->getLocalSupportPointWithMargin(direction);
}

// Return a local support point in a given direction without the object margin
inline Vector3 ProxySphereShape::getLocalSupportPointWithoutMargin(const Vector3& direction) {
    return mCollisionShape->getLocalSupportPointWithoutMargin(direction);
}

// Return the current object margin
inline decimal ProxySphereShape::getMargin() const {
    return mCollisionShape->getMargin();
}

// Return true if a point is inside the collision shape
inline bool ProxySphereShape::testPointInside(const Vector3& worldPoint) {
    const Transform localToWorld = mBody->getTransform() * mLocalToBodyTransform;
    const Vector3 localPoint = localToWorld.getInverse() * worldPoint;
    return mCollisionShape->testPointInside(localPoint);
}

}

#endif
