
#ifndef POINT_H
#define POINT_H

#include <iostream>
using namespace std;

class Point
{
private:
	float x_ = 0;
	float y_ = 0;
	float z_ = 0;
	float w_ = 0;

public:
	//Constructors
	Point() = default;

	Point(float x, float y, float z, float w);

	//Attribute getters
	float x() const;
	float y() const;
	float z() const;
	float w() const;

	//Operation methods
	bool operator==(const Point& other) const = default;
};

//Operation functions

//Displays a point in this format: 'Point(1, 2, 3)'
ostream& operator<<(ostream& os, const Point& pt);

//Must read point in this format: '<1, 2, 3>'
istream& operator>>(istream& is, Point& pt);


#endif /* POINT_H */