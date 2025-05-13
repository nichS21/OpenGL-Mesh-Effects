#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <array>
#include <iostream>
#include <optional>
#include "Point.h"
#include "Unit.h"
#include "Color.h"
#include "Ray.h"
#include "Hit.h"
#include "utils.h"
#include "Vertex.h"

class Triangle
{
public:
	//Data members
	Vertex v1;
	Vertex v2;
	Vertex v3;
	Unit flatNorm;						//unit vector perpendicular to the triangle
	Color color;						//solid color of whole triangle
	
	static bool smooth;					// static variable designating whether triangles have smooth or flat normal

	/*
	* Returns a hit if given ray intersects with the triangle
	*/
	optional<Hit> intersect(const Ray& ray) const;

	/*
	* Sets color of each vertex by creating a unit vector out of 
	* each vertices' coordinates and then using absolute values 
	* of the unit vectors components as color for the vertex
	*/
	void setSmooth(bool smooth);

	/*
	* Allows ability to resize a triangle (scale) and move it around (translate).
	* For our purposes, all given meshes are designed to fit in a 1x1x1 box.
	*/
	void resize(float scale, const Vector& v);

	// overloaded cout
	friend ostream& operator<<(ostream& os, const Triangle& t);
	
	// overloaded cin
	friend istream& operator>>(istream& is, Triangle& t);

	//to be called by meshes to draw their triangles
	void draw(const Material& mat) const;

};


#endif