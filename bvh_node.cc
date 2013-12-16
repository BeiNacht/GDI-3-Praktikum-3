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
	this->triangles.push_back(Triangle(&mesh, 0));
	Vec3f min = this->triangles.back().getAABBMin();
	Vec3f max = this->triangles.back().getAABBMax();

	for (unsigned int var = 1; var < faceIDs->size(); ++var)
	{
		this->triangles.push_back(Triangle(&mesh, var));

		for (int i = 0; i < 3; i++)
		{
			if (min[i] > this->triangles.back().getAABBMin()[i])
				min[i] = this->triangles.back().getAABBMin()[i];
			if (max[i] < this->triangles.back().getAABBMax()[i])
				max[i] = this->triangles.back().getAABBMax()[i];
		}
	}

	std::vector<Triangle> left;
	std::vector<Triangle> right;

	Vec3f middle = max;
	int axis = aabb.getLongestAxis();
	middle[axis] = (min[axis] + max[axis]) / 2;

	AABB half = AABB(min, middle);

	while (this->triangles.size() > 0)
	{
		if (half.inside(this->triangles.back().getCentroid()))
			left.push_back(this->triangles.back());
		else
			right.push_back(this->triangles.back());

		this->triangles.pop_back();
	}

    this->aabb = AABB(min, max);

	if (left.size() > MAX_LEAF_TRIANGLES)
	{
		this->left = new BVHNode();
		this->left->insert(&left);
	}

	if (right.size() > MAX_LEAF_TRIANGLES)
	{
		this->right = new BVHNode();
		this->right->insert(&right);
	}
}

void BVHNode::insert(std::vector<Triangle>* triangles)
{
	this->triangles.push_back(triangles->back());
	Vec3f min = this->triangles.back().getAABBMin();
	Vec3f max = this->triangles.back().getAABBMax();

	triangles->pop_back();

	for (unsigned int var = 2; var < triangles->size(); ++var)
	{
		this->triangles.push_back(triangles->back());
		triangles->pop_back();
		for (int i = 0; i < 3; i++)
		{
			if (min[i] > this->triangles.back().getAABBMin()[i])
				min[i] = this->triangles.back().getAABBMin()[i];
			if (max[i] < this->triangles.back().getAABBMax()[i])
				max[i] = this->triangles.back().getAABBMax()[i];
		}
	}

	std::vector<Triangle> left;
	std::vector<Triangle> right;

	Vec3f middle = max;
	int axis = aabb.getLongestAxis();
	middle[axis] = (min[axis] + max[axis]) / 2;

	AABB half = AABB(min, middle);

	while (this->triangles.size() > 0)
	{
		if (half.inside(this->triangles.back().getCentroid()))
			left.push_back(this->triangles.back());
		else
			right.push_back(this->triangles.back());

		this->triangles.pop_back();
	}

	this->aabb = AABB(min, max);

	if (left.size() > MAX_LEAF_TRIANGLES)
	{
		this->left = new BVHNode();
		this->left->insert(&left);
	}

	if (right.size() > MAX_LEAF_TRIANGLES)
	{
		this->right = new BVHNode();
		this->right->insert(&right);
	}
}

bool BVHNode::intersect(Ray const& ray, Intersection* intersection) const
{
    // TODO
    return false;
}
