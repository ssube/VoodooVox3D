#include "Octree.hpp"

namespace Common
{
    namespace Trees
    {
        OctreeNode::OctreeNode()
            : mLeaf(true), mOrigin(), mSize()
        {
            memset(mChildren, 0, sizeof(void*) * 8);
        }

        OctreeNode::~OctreeNode()
        {
            for ( uint8 child = 0; child < 8; ++child )
            {
                delete mChildren[child];
            }
        }

        uint32 Octree::GetObjectType()
        {
            return OBJECT_TYPE_OCTREENODE;
        }

        fvec3 OctreeNode::GetPosition()
        {
            return mOrigin;
        }

        void OctreeNode::SetPosition(fvec3 position)
        {
            mOrigin = position;
        }

        fvec3 OctreeNode::GetSize()
        {
            return mOrigin + mSize;
        }

        fvec3 OctreeNode::SetSize(fvec3 size)
        {
            mSize = size;
        }

        void OctreeNode::AddItem(GenericObject * object)
        {
            mLeaf = false;

            fvec3 halfSpace = mSize / 2;
            fvec3 nodePosition = object->GetPosition();

            // Find out which section it should be in
            uint8 section = 0;
            if ( nodePosition.x > halfSpace.x ) section |= 0x01;
            if ( nodePosition.y > halfSpace.y ) section |= 0x02;
            if ( nodePosition.z > halfSpace.z ) section |= 0x04;

            if ( mChildren[section] )
            {
                // Check if the existing object is a node
                if ( mChildren[section]->GetObjectType() == OBJECT_TYPE_OCTREENODE )
                {
                    OctreeNode * childNode = (OctreeNode*)mChildren[section];
                    childNode->AddItem(object);
                } else {
                    // Object or other leaf type, create node and shift
                    GenericObject * previous = mChildren[section];

                    OctreeNode * newnode = new OctreeNode();
                    mChildren[section] = newnode;

                    newnode->AddItem(previous);
                    newnode->AddItem(object);
                }
            } else {
                mChildren[section] = object;

                if ( object->GetObjectType() == OBJECT_TYPE_OCTREENODE )
                {
                    OctreeNode * childNode = (OctreeNode*)mChildren[section];

                    childNode->SetPosition(mOrigin + halfSpace);
                    childNode->SetSize(halfSpace);
                }
            }
        }

        void OctreeNode::Cull(ViewFrustrum * view, std::list<GenericObject*> * objectlist)
        {
            for ( uint32 child = 0; child < 8; ++child )
            {
                GenericObject * workingChild = mChildren[child];
                if ( workingChild )
                {
                    if ( workingChild->GetObjectType() == OBJECT_TYPE_OCTREENODE )
                    {
                        ((OctreeNode*)workingChild)->Cull(view, objectlist)
                    } else {
                        fvec3 workingPosition = workingChild->GetPosition();
                        int32 visible = view->Clip(workingPosition, workingPosition + workingChild->GetSize());

                        if ( visible >= 0 )
                        {
                            objectlist->push_back(workingChild);
                        }
                    }
                }
            }
        }

        Octree::Octree(uint32 expectedSize)
            : mRoot(0)
        {
            mRoot = new OctreeNode();
        }

        Octree::~Octree()
        {
            delete mRoot;
        }

        uint32 Octree::GetObjectType()
        {
            return OBJECT_TYPE_OCTREE;
        }

        fvec3 Octree::GetPosition()
        {
            return mRoot->GetPosition();
        }

        fvec3 Octree::GetSize()
        {
            return mRoot->GetSize();
        }

        void Octree::AddItem(GenericObject * object)
        {
            mRoot->AddItem(object);
        }

        void Octree::Cull(ViewFrustrum * view, std::list<GenericObject*> objectlist)
        {
            return mRoot->Cull(view, objectlist);
        }
    }
}
