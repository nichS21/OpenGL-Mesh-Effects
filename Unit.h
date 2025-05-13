#ifndef UNIT_H
#define UNIT_H

#include "Vector.h"
#include "Point.h"
#include <ostream>
using namespace std;

class Unit : public Vector
{
public:
	//Constructors
	Unit() = default; 

	Unit(float x, float y, float z);

	Unit(const Vector& v);

	Unit(const Point& p1, const Point& p2);

};

//Display unit vector with cout,
// format: Unit(1, 2, 3)
ostream& operator<<(ostream& os, const Unit& unit);

#endif /* UNIT_H */