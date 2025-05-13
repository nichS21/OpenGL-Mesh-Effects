#include "Ray.h"


#include <iostream>
using namespace std;

Ray::Ray(const Point& p, const Vector& d)
	:
	origin_(p), dir_(Unit(d)) //make sure that dir_ is a unit vector
{
}

Ray::Ray(const Point& p1, const Point& p2)
	:
	Ray(p1, Vector(p1, p2)) // reuse previous constructor
{
}

Point Ray::origin() const
{
	return origin_;
}

Unit Ray::dir() const
{
	return dir_;
}

//Return point on the ray that is t units away from the origin
Point Ray::point(float t) const
{
	return origin_ + (t * dir_);
}

ostream& operator<<(ostream& os, const Ray& ray)
{
	cout << ray.origin() << ", "
		<< ray.dir();
	
	return os;
}

