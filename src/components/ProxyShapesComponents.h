/********************************************************************************
* ReactPhysics3D physics library, http://www.reactphysics3d.com                 *
* Copyright (c) 2010-2018 Daniel Chappuis                                       *
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

#ifndef REACTPHYSICS3D_PROXY_SHAPES_COMPONENTS_H
#define REACTPHYSICS3D_PROXY_SHAPES_COMPONENTS_H

// Libraries
#include "mathematics/Transform.h"
#include "engine/Entity.h"
#include "containers/Map.h"
#include "collision/shapes/AABB.h"
#include "components/Components.h"

// ReactPhysics3D namespace
namespace reactphysics3d {

// Class declarations
class MemoryAllocator;
class EntityManager;
class AABB;
class CollisionShape;
class ProxyShape;

// Class ProxyShapesComponents
/**
 * This class represent the component of the ECS that contains data about the the proxy-shapes of the
 * different bodies. We also make sure that proxy shapes of sleeping entities (bodies) are
 * always stored at the end of the array.
 */
class ProxyShapesComponents : public Components {

    private:

        // -------------------- Constants -------------------- //

        const size_t COMPONENT_DATA_SIZE = sizeof(Entity) + sizeof(Entity) + sizeof(ProxyShape*) + sizeof(int) + sizeof(AABB) +
                sizeof(Transform) + sizeof(CollisionShape*) + sizeof(decimal) + sizeof(unsigned short) +
                sizeof(unsigned short);

        // -------------------- Attributes -------------------- //

        /// Index of the first component of a sleeping entity (sleeping components are stored at the end)
        uint32 mSleepingStartIndex;

        /// Array of entities of each component
        Entity* mBodiesEntities;

        /// Array of entities of each component
        Entity* mProxyShapesEntities;

        /// Array of pointer to the proxy-shapes
        ProxyShape** mProxyShapes;

        /// Ids of the proxy-shapes for the broad-phase algorithm
        // TODO : Try to change type to uint32
        int* mBroadPhaseIds;

        /// Local-space bounds of a proxy-shape
        AABB* mLocalBounds;

        /// Transform from local-space of the proxy-shape to the body-space of its body
        Transform* mLocalToBodyTransforms;

        /// Pointers to the collision shapes of the proxy-shapes
        CollisionShape** mCollisionShapes;

        /// Masses (in kilogramms) of the proxy-shapes
        decimal* mMasses;

        /// Array of bits used to define the collision category of this shape.
        /// You can set a single bit to one to define a category value for this
        /// shape. This value is one (0x0001) by default. This variable can be used
        /// together with the mCollideWithMaskBits variable so that given
        /// categories of shapes collide with each other and do not collide with
        /// other categories.
        unsigned short* mCollisionCategoryBits;

        /// Array of bits mask used to state which collision categories this shape can
        /// collide with. This value is 0xFFFF by default. It means that this
        /// proxy shape will collide with every collision categories by default.
        unsigned short* mCollideWithMaskBits;

        // -------------------- Methods -------------------- //

        /// Destroy a component at a given index
        void destroyComponent(uint32 index);

        /// Move a component from a source to a destination index in the components array
        void moveComponentToIndex(uint32 srcIndex, uint32 destIndex);

        /// Swap two components in the array
        void swapComponents(uint32 index1, uint32 index2);

    public:

        /// Structure for the data of a proxy shape component
        struct ProxyShapeComponent {

            Entity bodyEntity;
            ProxyShape* proxyShape;
            int broadPhaseId;
            AABB localBounds;
            Transform localToBodyTransform;
            CollisionShape* collisionShape;
            decimal mass;
            unsigned short collisionCategoryBits;
            unsigned short collideWithMaskBits;

            /// Constructor
            ProxyShapeComponent(Entity bodyEntity, ProxyShape* proxyShape, int broadPhaseId, AABB localBounds, Transform localToBodyTransform,
                                CollisionShape* collisionShape, decimal mass, unsigned short collisionCategoryBits,
                                unsigned short collideWithMaskBits)
                 :bodyEntity(bodyEntity), proxyShape(proxyShape), broadPhaseId(broadPhaseId), localBounds(localBounds), localToBodyTransform(localToBodyTransform),
                  collisionShape(collisionShape), mass(mass), collisionCategoryBits(collisionCategoryBits), collideWithMaskBits(collideWithMaskBits) {

            }
        };

        // -------------------- Methods -------------------- //

        /// Constructor
        ProxyShapesComponents(MemoryAllocator& allocator);

        /// Destructor
        virtual ~ProxyShapesComponents();

        /// Allocate memory for a given number of components
        void allocate(uint32 nbComponentsToAllocate);

        /// Add a component
        void addComponent(Entity proxyShapeEntity, bool isSleeping, const ProxyShapeComponent& component);

        /// Remove a component at a given index
        void removeComponent(Entity proxyShapeEntity);

        /// Notify if a given entity is sleeping or not
        void setIsEntitySleeping(Entity entity, bool isSleeping);

        /// Return the mass of a proxy-shape
        decimal getMass(Entity proxyShapeEntity) const;

        /// Return a pointer to a given proxy-shape
        ProxyShape* getProxyShape(Entity proxyShapeEntity) const;

        /// Return the local-to-body transform of a proxy-shape
        const Transform& getLocalToBodyTransform(Entity proxyShapeEntity) const;

        /// Set the local-to-body transform of a proxy-shape
        void setLocalToBodyTransform(Entity proxyShapeEntity, const Transform& transform);

        /// Return a pointer to the collision shape of a proxy-shape
        CollisionShape* getCollisionShape(Entity proxyShapeEntity) const;

        /// Return the broad-phase id of a given proxy shape
        int getBroadPhaseId(Entity proxyShapeEntity) const;

        /// Set the broad-phase id of a given proxy shape
        void setBroadPhaseId(Entity proxyShapeEntity, int broadPhaseId);

        /// Return the collision category bits of a given proxy-shape
        unsigned short getCollisionCategoryBits(Entity proxyShapeEntity) const;

        /// Set the collision category bits of a given proxy-shape
        void setCollisionCategoryBits(Entity proxyShapeEntity, unsigned short collisionCategoryBits);

        /// Return the "collide with" mask bits of a given proxy-shape
        unsigned short getCollideWithMaskBits(Entity proxyShapeEntity) const;

        /// Set the "collide with" mask bits of a given proxy-shape
        void setCollideWithMaskBits(Entity proxyShapeEntity, unsigned short collideWithMaskBits);
};

// Return the mass of a proxy-shape
inline decimal ProxyShapesComponents::getMass(Entity proxyShapeEntity) const {

   assert(mMapEntityToComponentIndex.containsKey(proxyShapeEntity));

   return mMasses[mMapEntityToComponentIndex[proxyShapeEntity]];
}

// Return a pointer to a given proxy-shape
inline ProxyShape* ProxyShapesComponents::getProxyShape(Entity proxyShapeEntity) const {

   assert(mMapEntityToComponentIndex.containsKey(proxyShapeEntity));

   return mProxyShapes[mMapEntityToComponentIndex[proxyShapeEntity]];
}

// Return the local-to-body transform of a proxy-shape
inline const Transform& ProxyShapesComponents::getLocalToBodyTransform(Entity proxyShapeEntity) const {

   assert(mMapEntityToComponentIndex.containsKey(proxyShapeEntity));

   return mLocalToBodyTransforms[mMapEntityToComponentIndex[proxyShapeEntity]];
}

// Set the local-to-body transform of a proxy-shape
inline void ProxyShapesComponents::setLocalToBodyTransform(Entity proxyShapeEntity, const Transform& transform) {

   assert(mMapEntityToComponentIndex.containsKey(proxyShapeEntity));

   mLocalToBodyTransforms[mMapEntityToComponentIndex[proxyShapeEntity]] = transform;
}

// Return a pointer to the collision shape of a proxy-shape
inline CollisionShape* ProxyShapesComponents::getCollisionShape(Entity proxyShapeEntity) const {

   assert(mMapEntityToComponentIndex.containsKey(proxyShapeEntity));

    return mCollisionShapes[mMapEntityToComponentIndex[proxyShapeEntity]];
}

// Return the broad-phase id of a given proxy shape
inline int ProxyShapesComponents::getBroadPhaseId(Entity proxyShapeEntity) const {

    assert(mMapEntityToComponentIndex.containsKey(proxyShapeEntity));

    return mBroadPhaseIds[mMapEntityToComponentIndex[proxyShapeEntity]];
}

// Set the broad-phase id of a given proxy shape
inline void ProxyShapesComponents::setBroadPhaseId(Entity proxyShapeEntity, int broadPhaseId) {

    assert(mMapEntityToComponentIndex.containsKey(proxyShapeEntity));

    mBroadPhaseIds[mMapEntityToComponentIndex[proxyShapeEntity]] = broadPhaseId;
}

// Return the collision category bits of a given proxy-shape
inline unsigned short ProxyShapesComponents::getCollisionCategoryBits(Entity proxyShapeEntity) const {

    assert(mMapEntityToComponentIndex.containsKey(proxyShapeEntity));

    return mCollisionCategoryBits[mMapEntityToComponentIndex[proxyShapeEntity]];
}

// Return the "collide with" mask bits of a given proxy-shape
inline unsigned short ProxyShapesComponents::getCollideWithMaskBits(Entity proxyShapeEntity) const {

    assert(mMapEntityToComponentIndex.containsKey(proxyShapeEntity));

    return mCollideWithMaskBits[mMapEntityToComponentIndex[proxyShapeEntity]];
}

// Set the collision category bits of a given proxy-shape
inline void ProxyShapesComponents::setCollisionCategoryBits(Entity proxyShapeEntity, unsigned short collisionCategoryBits) {

    assert(mMapEntityToComponentIndex.containsKey(proxyShapeEntity));

    mCollisionCategoryBits[mMapEntityToComponentIndex[proxyShapeEntity]] = collisionCategoryBits;
}

// Set the "collide with" mask bits of a given proxy-shape
inline void ProxyShapesComponents::setCollideWithMaskBits(Entity proxyShapeEntity, unsigned short collideWithMaskBits) {

    assert(mMapEntityToComponentIndex.containsKey(proxyShapeEntity));

    mCollideWithMaskBits[mMapEntityToComponentIndex[proxyShapeEntity]] = collideWithMaskBits;
}

}

#endif
