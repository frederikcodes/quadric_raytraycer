#include "acceleration/OctreeNode.h"

bool OctreeNode::isLeaf() const
{
    for (int i = 0; i < 8; i++)
    {
        if (children[i])
        {
            return false;
        }
    }

    return true;
}
