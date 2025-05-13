#include "Unit.h"

#include <iostream>
using namespace std;

Unit::Unit(float x, float y, float z)
{
	Vector v(x, y, z);
	Vector unitV = v / (v.length());

	x_ = unitV.x();
	y_ = unitV.y();
	z_ = unitV.z();
}

Unit::Unit(const Vector& v)
	:
	Unit(v.x(), v.y(), v.z())
{
}

Unit::Unit(const Point& p1, const Point& p2)
	:
	Unit(Vector(p1, p2))
{
}

//Format: 'Unit(1, 2, 3)'
ostream& operator<<(ostream& os, const Unit& unit)
{
	cout << "Unit(" << unit.x() << ", " << unit.y() << ", " << unit.z() << ")";
	
	return os;
}