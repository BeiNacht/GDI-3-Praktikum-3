#include <limits>
#include <stdexcept>

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "visual.h"
#include "bvh_node.h"

using namespace std;

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
	std::vector<Triangle> left;
	std::vector<Triangle> right;
	Vec3f min;
	Vec3f max;
	Vec3f middle;
	int axis;
	AABB half;


	//erstelle erste Dreieck und setzte Baseline für min/max des AABB
	this->triangles.push_back(Triangle(&mesh, 0));
	min = this->triangles.back().getAABBMin();
	max = this->triangles.back().getAABBMax();


	//füge alle Dreiecke ein und prüfe ob neues min/max gefunden wurde
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

    this->aabb = AABB(min, max);


	//prüfe welche Achse die längste ist und teile AABB in der Mitte an dieser
	middle = max;
	axis = aabb.getLongestAxis();
	middle[axis] = (min[axis] + max[axis]) / 2;
	half = AABB(min, middle);


	//teile Dreiecke in Dreiecke die im neuen AABB sind und Dreicke die im anderen AABB sind
	while (this->triangles.size() > 0)
	{
		if (half.inside(this->triangles.back().getCentroid()))
			left.push_back(this->triangles.back());
		else
			right.push_back(this->triangles.back());

		this->triangles.pop_back();
	}


	//erstelle Kinder
	if (left.size() > MAX_LEAF_TRIANGLES)
	{
		this->left = new BVHNode();
		this->left->insert(&left);}

	if (right.size() > MAX_LEAF_TRIANGLES)
	{
		this->right = new BVHNode();
		this->right->insert(&right);}
}

void BVHNode::insert(std::vector<Triangle>* triangles)
{
	std::vector<Triangle> left,right;
	Vec3f min,max,middle;
	AABB half;
	int axis;


	//prüfe Triangle größe
	if (triangles->size() == 0)
		return;


	//erstelle erste Dreieck und setzte Baseline für min/max des AABB
	this->triangles.push_back(triangles->back());
	min = this->triangles.back().getAABBMin();
	max = this->triangles.back().getAABBMax();
	triangles->pop_back();


	//füge alle übergebenen Dreiecke ein und prüfe ob neues min/max gefunden wurde
	while(triangles->size() > 0)
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


	//erstelle AAB und prüfe ob der aktuelle Knoten
	this->aabb = AABB(min, max);
	if (this->triangles.size() <= MAX_LEAF_TRIANGLES)
		return;



	//prüfe welche Achse die längste ist und teile AABB in der Mitte an dieser
	middle = max;
	axis = aabb.getLongestAxis();
	middle[axis] = (min[axis] + max[axis]) / 2;
	half = AABB(min, middle);



	//teile Dreiecke in Dreiecke die im neuen AABB sind und Dreicke die im anderen AABB sind
	while (this->triangles.size() > 0)
	{
		if (half.inside(this->triangles.back().getCentroid()))
			left.push_back(this->triangles.back());
		else
			right.push_back(this->triangles.back());

		this->triangles.pop_back();
	}

	//prüfe ob trotz des Versuchs aufteilen kein Dreieck in
	if (left.size() == 0)
	{
		left.push_back(right.back());
		right.pop_back();}
	if (right.size() == 0)
	{
		right.push_back(left.back());
		left.pop_back();}


	//erstelle Kinder
	this->left = new BVHNode();
	this->left->insert(&left);

	this->right = new BVHNode();
	this->right->insert(&right);
}

bool BVHNode::intersect(Ray const& ray, Intersection* intersection) const
{
	bool hitl = false;
	bool hitr = false;

	//prüft ob der Strahl den aktuellen Knoten/dessen Kinder trifft
	if (this->aabb.intersect(ray))
	{
		//Blatt, prüfe für jedes Dreieck ob der Strahl es trifft
		if (left == NULL && right == NULL)
		{
			for (unsigned int var = 0; var < triangles.size(); ++var)
				if (triangles.at(var).intersect(ray, intersection))
					return true;
		}
		//kein Blatt, rek. Aufruf
		else
		{
			hitl = left->intersect(ray, intersection);
			hitr = right->intersect(ray, intersection);

			return hitl || hitr;
		}

	}
    return false;
}
