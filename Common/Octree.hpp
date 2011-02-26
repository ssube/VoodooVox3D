#pragma once

#include "CommonTypes.hpp"
#include <list>

#define OBJECT_TYPE_OCTREE     0x01
#define OBJECT_TYPE_OCTREENODE 0x02

namespace Common
{
    namespace Trees
    {
        class COMMON_API OctreeNode
            : public GenericObject
        {
        public:
            OctreeNode();
            ~OctreeNode();

            virtual uint32 GetObjectType();

            virtual fvec3 GetPosition();
            virtual void  SetPosition(fvec3 position);

            virtual fvec3 GetSize();
            virtual void SetSize(fvec3 size);

            virtual void AddItem(GenericObject * object);

            virtual void Cull(GenericCamera * camera, std::list<GenericObject*> * objectlist);

        private:
            fvec3 mOrigin;
            fvec3 mSize;

            bool mLeaf;
            GenericObject * mChildNodes[8];
            std::list<GenericObject*> mChildObjects;
        };

        class COMMON_API Octree
            : public GenericObject
        {
        public:
            Octree(fvec3 origin, fvec3 size);
            ~Octree();

            virtual uint32 GetObjectType();

            virtual fvec3 GetPosition();
            virtual fvec3 GetSize();

            virtual void AddItem(GenericObject * object);

            // Returns count and array in objectlist
            virtual void Cull(GenericCamera * camera, std::list<GenericObject*> * objectlist);

        private:
            OctreeNode * mRoot;
        };
    }
}
