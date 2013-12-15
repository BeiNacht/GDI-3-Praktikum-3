#include <limits>
#include <stdexcept>
//test
#include <limits>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "visual.h"
#include "bvh_node.h"

BVHNode::BVHNode()
{
}

BVHNode::~BVHNode()
{
    // TODO
}

void BVHNode::insert(Mesh const& mesh, std::vector<unsigned int>* faceIDs)
{
	Vec3f min;
	Vec3f max;
	Vec3f tempMin;
	Vec3f tempMax;
    for (unsigned int i = 0; i < 10; i++)
    {
    	triangles.push_back(Triangle(&mesh, i));
    	tempMin = triangles.back().getAABBMin();
    	tempMax = triangles.back().getAABBMax();

    	if (i == 0)
    	{
    		min = tempMin;
    		max = tempMax;
    	}
    	else
    	{
			if (tempMin.length() < min.length())
				min = tempMin;
			if (tempMax.length() > max.length())
				max = tempMax;
    	}
    }

    this->aabb = AABB(min, max);
    //this->left->insert(mesh, faceIDs);
    //this->right->insert(mesh, faceIDs);
    //std::cout << "  Dreiecks: " << triangles.size() << std::endl;
}

bool BVHNode::intersect(Ray const& ray, Intersection* intersection) const
{
    // TODO
    return false;
}
