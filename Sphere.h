#ifndef SPHERE_H
#define SPHERE_H

#include "Point.h"
#include "Ray.h"
#include "Material.h"
#include "Hit.h"
#include "Shape.h"
#include "Image.h"
#include <optional>


class Sphere : public Shape
{
private:
	Point center;
	float radius;

public:
	/*
	* Default constructor which creates a unit Sphere (center = origin, radius = 1)
	*/
	Sphere();

	/*
	* Creates a sphere with given center and radius
	*/
	Sphere(const Point& cn, float r);

	/*
	* Return C++ 'optional' of 'Hit' object representing
	* the point of intersection between the sphere and
	* a given ray.
	*/
	optional<Hit> intersect(const Ray& ray) const override;

	/*
	* Method to determine if given t results in a viable hit.
	* Returns nullopt if not viable
	*/
	optional<Hit> viableT(float t, const Ray& ray) const override;

	/*
	* Override from Shape.h
	*/
	pair<float, float> texel_uv(const Point& pt) const override;

	/*
	* Override from Shape.h
	*/
	Unit bumpNormal(const Point& pt, const Unit& N, float u, float v) const override;

	/*
	* Override from Shape.h
	*/
	Color selectColor(float u, float v) const;

	/*
	* Overide from Shape.h
	*/
	bool isVisible(float u, float v) const;

	/*
	* Overload of cout for Sphere, with access to private data members.
	* Format: 'Sphere: Point(1,2,3) 456 Mat(ka,kd,ks,n)'
	*/
	friend ostream& operator<<(ostream& os, const Sphere& sphere);

	/*
	* Overload of cin for Sphere, with access to private data members.
	* Note: assume 'sphere' word has already been consumed when this method is called,
	* within Scene class
	*/
	friend istream& operator>>(istream& is, Sphere& sphere);

	/*
	* Inherited from Shape.h
	*/
	void draw() const override;
};

#endif