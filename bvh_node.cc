#include <limits>
#include <stdexcept>
//test
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "visual.h"
#include "bvh_node.h"

BVHNode::BVHNode()
{
	this->left = NULL;
	this->right = NULL;
}

BVHNode::~BVHNode()
{
    // TODO
}

void BVHNode::insert(Mesh const& mesh, std::vector<unsigned int>* faceIDs)
{
	triangles.push_back(Triangle(&mesh, 0));
	Vec3f min = triangles.back().getAABBMin();
	Vec3f max = triangles.back().getAABBMax();

	for (unsigned int var = 1; var < faceIDs->size(); ++var)
	{
		triangles.push_back(Triangle(&mesh, var));

		for (int i = 0; i < 3; i++)
		{
			if (min[i]>triangles.back().getAABBMin()[i])
				min[i] = triangles.back().getAABBMin()[i];
			if (max[i]<triangles.back().getAABBMax()[i])
				max[i] = triangles.back().getAABBMax()[i];
		}
	}

	std::vector<Triangle> left;
	std::vector<Triangle> right;

	Vec3f middle = max;
	int axis = aabb.getLongestAxis();
	middle[axis] = (min[axis] + max[axis]) / 2;

	AABB half = AABB(min, middle);

	while (triangles.size() > 0)
	{
		if (half.inside(triangles.back().getCentroid()))
			left.push_back(triangles.back());
		else
			left.push_back(triangles.back());

		triangles.pop_back();
	}

    this->aabb = AABB(min, max);
}

bool BVHNode::intersect(Ray const& ray, Intersection* intersection) const
{
    // TODO
    return false;
}
