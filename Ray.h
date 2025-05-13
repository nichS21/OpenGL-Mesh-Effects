#ifndef RAY_H
#define RAY_H

#include "Point.h"
#include "Vector.h"
#include "Unit.h"

class Ray
{
private:
	Point origin_;
	Unit dir_;		//vector normalized to be of unit length

public:
	//Constructors
	Ray(const Point& p, const Vector& d);

	Ray(const Point& p1, const Point& p2);

	//Accessor methods
	Point origin() const;
	Unit dir() const;

	//Returns point on ray that is 't' units away from the origin
	Point point(float t) const;

};

//cout overload
ostream& operator<<(ostream& os, const Ray& r);

#endif /* RAY_H */