#ifndef VECTOR_H
#define VECTOR_H

#include "Point.h"

class Vector 
{
protected:			//protected so can be accessed by subclasses, but no one else
	float x_ = 0;
	float y_ = 0;
	float z_ = 0;

public:
	//Constructors
	Vector() = default;

	Vector(float x, float y, float z);

	Vector(const Point& p1, const Point& p2);

	//Creates a vector toward the given point (from the origin)
	Vector(const Point& pt);

	//Attribute getters
	float x() const;
	float y() const;
	float z() const;

	//Methods
	bool operator==(const Vector& other) const = default;

	float length() const;

	float sq_length() const;

	Vector operator-(const Vector& v2) const;

	//Vector operator-() const; //inversion as method

	//Scalar multiplication: <x,y,z> * n
	Vector operator*(float scalar) const;

	//Cross product, not same as dot product
	Vector cross(const Vector& v2) const;

	//Returns point representation of a vector
	Point point() const;

};

//Functions independent of class

//Dot product of two vectors
float dot(const Vector& v1, const Vector& v2);

Vector operator+(const Vector& v1, const Vector& v2);

Vector operator-(const Vector& vector); //inversion as function

//Scalar multiplication: n * <x,y,z>
Vector operator*(float scalar, const Vector& vector);

Vector operator/(const Vector& vector, float scalar);

//Vector inversion; i.e. -<1,3,4> == <-1,-3,-4>
Vector operator-(const Vector& vector);

//Point vector add (add vector to a point, returning a point)
Point operator+(const Point& pt, const Vector& vr);

//Point vector subtract (subtract vector from a point, returning a point)
Point operator-(const Point& pt, const Vector& vr);

//Displays a vector in this format: 'Vector(1, 2, 3)'
ostream& operator<<(ostream& os, const Vector& vr);

istream& operator>>(istream& is, Vector& vr);


#endif /* VECTOR_H */