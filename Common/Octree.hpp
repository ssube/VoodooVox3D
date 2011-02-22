#pragma once

#include "CommonTypes.hpp"
#include <list>

#define OBJECT_TYPE_OCTREE     0x01
#define OBJECT_TYPE_OCTREENODE 0x02

namespace Common
{
    namespace Trees
    {
        class OctreeNode
            : public GenericObject
        {
            OctreeNode();
            ~OctreeNode();

            virtual uint32 GetObjectType();

            virtual fvec3 GetPosition();
            virtual void  SetPosition(fvec3 position);

            virtual void GetSize();
            virtual fvec3 SetSize(fvec3 size);

            virtual void AddItem(GenericObject * object);

            virtual void Cull(ViewFrustrum * view, std::list<GenericObject*> objectlist);

        private:
            fvec3 mOrigin;
            fvec3 mSize;

            bool mLeaf;
            GenericObject * mChildren[8];
        };

        class Octree
            : public GenericObject
        {
            Octree();
            ~Octree();

            virtual uint32 GetObjectType();

            virtual fvec3 GetPosition();
            virtual fvec3 GetSize();

            virtual void AddItem(GenericObject * object);

            // Returns count and array in objectlist
            virtual void Cull(ViewFrustrum * view, std::list<GenericObject*> objectlist);

        private:
            OctreeNode * mRoot;
        };
    }
}
