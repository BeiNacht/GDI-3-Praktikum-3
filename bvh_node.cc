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
	this->left = new BVHNode();
	this->right = new BVHNode();
}

BVHNode::~BVHNode()
{
    // TODO
}

void BVHNode::insert(Mesh const& mesh, std::vector<unsigned int>* faceIDs)
{
    for (unsigned int i = 0; i < 10; i++)
    {
    	triangles.push_back(Triangle(&mesh, i));
    }

    this->left->insert(mesh, faceIDs);
    this->right->insert(mesh, faceIDs);
    std::cout << "  Dreiecks: " << triangles.size() << std::endl;
}

bool BVHNode::intersect(Ray const& ray, Intersection* intersection) const
{
    // TODO
    return true;
}
