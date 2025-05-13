#ifndef VERTEX_H
#define VERTEX_H

#include "Point.h"
#include "Color.h"
#include "Unit.h"

struct Vertex
{
	//Data members
	Point point;
	Color vColor;
	Unit vNormal;
};


#endif