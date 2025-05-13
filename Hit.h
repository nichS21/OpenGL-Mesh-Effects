#ifndef HIT_H
#define HIT_H

#include "Point.h"
#include "Unit.h"
#include "Color.h"
#include "Material.h"

/*
* Representation of point of intersection between a ray and a shape.
* 
* Note-- use default constructor (from compiler) to make a 'Hit' object:
*		'Hit hit{order,of,data,members};'
*/
struct Hit 
{
	//Data members
	Point inter;	// point of intersection
	Unit normal;	// unit normal vector at point of intersection
	Color color;	// color of object at point of intersection (INTRINSIC color of object)
	float t;		// distance along ray at which interseciton occured
	Material mat;	// material of the object at intersection

	float u;
	float v;		// u,v represent coordinates within an image, to map it to an object
};

#endif