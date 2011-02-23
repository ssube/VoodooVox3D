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

        uint32 OctreeNode::GetObjectType()
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
            return mSize;
        }

        void OctreeNode::SetSize(fvec3 size)
        {
            mSize = size;
        }

        void OctreeNode::AddItem(GenericObject * object)
        {
            fvec3 halfSize = mSize / 2;
            fvec3 halfSpace = mOrigin + halfSize;
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
                    mLeaf = false;

                    // Object or other leaf type, create node and shift
                    GenericObject * previous = mChildren[section];

                    OctreeNode * newNode = new OctreeNode();
                    mChildren[section] = newNode;

                    fvec3 childPos = mOrigin;
                    if ( section & 0x01 ) { childPos.x += halfSize.x; }
                    if ( section & 0x02 ) { childPos.y += halfSize.y; }
                    if ( section & 0x04 ) { childPos.z += halfSize.z; }

                    newNode->SetPosition(childPos);
                    newNode->SetSize(halfSize);

                    newNode->AddItem(previous);
                    newNode->AddItem(object);
                }
            } else {
                mChildren[section] = object;

                if ( object->GetObjectType() == OBJECT_TYPE_OCTREENODE )
                {
                    mLeaf = false;

                    OctreeNode * newNode = (OctreeNode*)mChildren[section];

                    fvec3 childPos = mOrigin;
                    if ( section & 0x01 ) { childPos.x += halfSize.x; }
                    if ( section & 0x02 ) { childPos.y += halfSize.y; }
                    if ( section & 0x04 ) { childPos.z += halfSize.z; }

                    newNode->SetPosition(childPos);
                    newNode->SetSize(halfSize);
                }
            }
        }

        void OctreeNode::Cull(GenericCamera * camera, std::list<GenericObject*> * objectlist)
        {
            for ( uint32 child = 0; child < 8; ++child )
            {
                GenericObject * workingChild = mChildren[child];
                if ( workingChild )
                {
                    fvec3 workingPosition = workingChild->GetPosition();
                    int32 visible = camera->Clip(workingPosition, workingPosition + workingChild->GetSize());

                    if ( visible >= 0 )
                    {
                        if ( workingChild->GetObjectType() == OBJECT_TYPE_OCTREENODE )
                        {
                            ((OctreeNode*)workingChild)->Cull(camera, objectlist);
                        } else {
                            objectlist->push_back(workingChild);
                        }
                    }
                }
            }
        }

        Octree::Octree(fvec3 origin, fvec3 size)
            : mRoot(0)
        {
            mRoot = new OctreeNode();
            mRoot->SetPosition(origin);
            mRoot->SetSize(size);
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

        void Octree::Cull(GenericCamera * camera, std::list<GenericObject*> * objectlist)
        {
            objectlist->clear();

            return mRoot->Cull(camera, objectlist);
        }
    }
}
