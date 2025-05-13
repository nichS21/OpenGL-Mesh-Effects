#include "Vector.h"

#include <iostream>
#include <cmath>
using namespace std;

Vector::Vector(float x, float y, float z)
	:
	x_(x), y_(y), z_(z)
{
}

/*
* Create a vector from a starting and terminal point.
* Is equivalent to one anchored at the origin, since vectors are 
* only a magnitude and direction.
*/
Vector::Vector(const Point& p1, const Point& p2) 
	:
	Vector(p2.x() - p1.x(), p2.y() - p1.y(), p2.z() - p1.z())
{
}

Vector::Vector(const Point& pt)
	:
	Vector(pt.x(), pt.y(), pt.z())
{
}

float Vector::x() const
{
	return x_;
}

float Vector::y() const
{
	return y_;
}

float Vector::z() const
{
	return z_;
}

float Vector::length() const
{
	float innerEQ = pow(x(), 2) + pow(y(), 2) + pow(z(), 2);
	return sqrt(innerEQ);
}

float Vector::sq_length() const
{
	return pow(x(), 2) + pow(y(), 2) + pow(z(), 2);
}

//Dot product of two vectors
float dot(const Vector& v1, const Vector& v2)
{
	return (v1.x() * v2.x()) + (v1.y() * v2.y()) + (v1.z() * v2.z());
}

Vector Vector::cross(const Vector& v2) const
{
	float x = (this->y() * v2.z()) - (this->z() * v2.y());
	float y = (this->z() * v2.x()) - (this->x() * v2.z());
	float z = (this->x() * v2.y()) - (this->y() * v2.x());

	return Vector(x, y, z);
}

Point Vector::point() const
{
	return Point(x_, y_, z_, 1);
}

Vector operator+(const Vector& v1, const Vector& v2)
{
	float newX = v1.x() + v2.x();
	float newY = v1.y() + v2.y();
	float newZ = v1.z() + v2.z();

	Vector newVec(newX, newY, newZ);

	return newVec;
}

Vector Vector::operator-(const Vector& v2) const
{
	float newX = x_ - v2.x();
	float newY = y_ - v2.y();
	float newZ = z_ - v2.z();

	Vector newVec(newX, newY, newZ);

	return newVec;
}

// Vector inversion as method
//Vector Vector::operator-() const
//{
//	Vector newVec(-x_, -y_, -z_);
//
//	return newVec;
//}

//Vector inversion as function
Vector operator-(const Vector& vector)
{
	Vector newVec(-vector.x(), -vector.y(), -vector.z());

	return newVec;
}

//Scalar multipliation: <x,y,z> * n
Vector Vector::operator*(float scalar) const
{
	float newX = x_ * scalar;
	float newY = y_ * scalar;
	float newZ = z_ * scalar;

	Vector newVec(newX, newY, newZ);

	return newVec;
}

//Scalar multiplication: n * <x,y,z>
Vector operator*(float scalar, const Vector& vector)
{
	//multiplication is communitative -> doesn't matter the order -> reuse other function
	return vector * scalar;
}

Vector operator/(const Vector& vector, float scalar)
{
	float newX = vector.x() / scalar;
	float newY = vector.y() / scalar;
	float newZ = vector.z() / scalar;

	Vector newVec(newX, newY, newZ);

	return newVec;
}

//Point vector add (add vector to a point, returning a point)
// vector + point not supported
Point operator+(const Point& pt, const Vector& vr) 
{
	float newX = pt.x() + vr.x();
	float newY = pt.y() + vr.y();
	float newZ = pt.z() + vr.z();

	Point newPt(newX, newY, newZ, pt.w());

	return newPt;
}

//Point vector subtract (subtract vector from a point, returning a point)
// vector - point not supported
Point operator-(const Point& pt, const Vector& vr)
{
	float newX = pt.x() - vr.x();
	float newY = pt.y() - vr.y();
	float newZ = pt.z() - vr.z();

	Point newPt(newX, newY, newZ, pt.w());

	return newPt;
}

ostream& operator<<(ostream& os, const Vector& vr)
{
	cout << "Vector(" << vr.x() << ", " << vr.y() << ", " << vr.z() << ")";

	return os;
}

istream& operator>>(istream& is, Vector& v)
{
	char ch;
	float x, y, z;

	is >> ch;		// read '<'
	is >> x >> ch;	// read coord and comma
	is >> y >> ch;
	is >> z;
	is >> ch;		// read '>'

	v = Vector(x, y, z); // make the point from user input

	return is;
}

