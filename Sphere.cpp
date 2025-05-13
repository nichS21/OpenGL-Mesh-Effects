#include "Sphere.h"
#include "utils.h"
#include <string>
using namespace std;

void Sphere::draw() const
{
														// sphere example
	glPushMatrix();										// 1. isolate ... (save current matrix)

	mat.load(color);									// 2. draw object (sphere of radius r, x slices, y stacks)


	glTranslatef(transComp[0], transComp[1], transComp[2]);				// translate the current matrix before drawing sphere

	glRotatef(rotateComp[1], 0, 1, 0);
	glRotatef(rotateComp[2], 0, 0, 1);
	glRotatef(rotateComp[0], 1, 0, 0);									// rotate the current matrix along each axis

	glScalef(scaleComp[0], scaleComp[1], scaleComp[2]);					// scale up the current matrix


	glutSolidSphere(radius, 80, 50);

	glPopMatrix();										// 3. undo isolate, go back to original transformation state (restore previous, untranslated matrix)
}


Sphere::Sphere()
{
	radius = 0.5;
	center = Point(0,0,0, 1);

	//hardcode baseline material coefficients
	mat.ka = 0.3;
	mat.kd = 0.5;
	mat.ks = 0.7;
	mat.n = 70;

	//randomly choose color components using 'utils.h'
	float rd = genFloat();
	float gn = genFloat();
	float be = genFloat();
	color = Color(rd, gn, be);
}

Sphere::Sphere(const Point& cn, float r )
{
	radius = r;
	center = cn;
}

optional<Hit> Sphere::intersect(const Ray& ray) const
{
	/*
	* Solve for t to get point of intersection using portion
	* of sphere equation with ray parametric substituted in.
	* Note that one you have solved for one of x,y,z, you can copy and 
	* have the expanded forms for the other two.
	* 
	* If no intersect, end and return nothing, else continue
	*/
	double a = 1;	//dot(ray.dir(), ray.dir());				//since ray's direction is always a unit vector, should always equal 1

	
	Vector centerOrigin(ray.origin().x() - center.x(),			//represent ray origin - center as a vector
					    ray.origin().y() - center.y(),
						ray.origin().z() - center.z());
	double b =  2 * dot(ray.dir(), centerOrigin);

	double c = dot(centerOrigin, centerOrigin) - (radius * radius);



	//calculate discriminant (get t values if are any)
	pair<float, float> tValues = quadratic(a, b, c);
	

	//go through t's, and return first valid hit
	optional<Hit> hit = viableT(tValues.first, ray);
	if (hit) return hit;								//first hit valid, return

	return viableT(tValues.second, ray);				//second hit either valid or invalid	
}


optional<Hit> Sphere::viableT(float t, const Ray& ray) const
{
	//negative t (or t too close to object) = immediate fail
	if (t < ZERO) return {};

	//otherwise, t is valid for a sphere
	Point intersect = ray.point(t);


	//check if object isVisible (for masking purposes)
	auto [u, v] = texel_uv(intersect);

	if (isVisible(u, v)) 
	{
		//get either solid color of shape or color of its texture at intersection
		Color obColor = selectColor(u, v);


		Unit normal(center, intersect);										//unit vector of Normal vector
		normal = bumpNormal(intersect, normal, u, v);						// if bump map for this shape, perturb normal, otherwise normal remains the same

		// create Hit object and return it
		return Hit{ intersect,normal,obColor,t,mat };
	}

	return {};																//intersection is not visible -> do not return a hit
}

pair<float, float> Sphere::texel_uv(const Point& pt) const
{
	// correct point to be at origin
	Point corrP(pt.x() - center.x(), pt.y() - center.y(), pt.z() - center.z(), 1);

	float r = sqrt((corrP.x() * corrP.x()) + 
			       (corrP.y() * corrP.y()) + 
				   (corrP.z() * corrP.z()));

	float theta = atan2(corrP.x(), corrP.z());		// range [-PI, PI]
	float phi = acos(corrP.y() / r);				// range [0, PI]


	//convert theta and phi's ranges to [0..1]
	theta = theta + PI;										// range [0, 2PI]
	float u = theta / (2 * PI);								// range [0, 1]

	float v = phi / PI;										// range [0, 1]


	//return values
	return pair<float, float> {u, v};
}

Unit Sphere::bumpNormal(const Point& pt, const Unit& N, float u, float v) const
{
	if (bumpMap == nullptr) return N;		//no bump map, return regular normal

	//else was a bump map
	//compute U, V, then perturbed normal
	auto [du, dv] = bumpMap->gradient(u, v);
	
	Vector yAxis(0, 1, 0);
	Vector U = yAxis.cross(N);				// U always perp to y axis and normal (right hand rule)
	Vector V = N.cross(U);

	Vector norm = N + du * U + dv * V;
	return Unit(norm);
}

Color Sphere::selectColor(float u, float v) const
{
	//if no texture, just return solid color
	if (texture == nullptr) return color;

	//is texture, so find color at given coordinates in texure map
	return texture->rgb_uv(u, v);
}

bool Sphere::isVisible(float u, float v) const
{
	if (this->mask == nullptr) return true;		//if no mask, point is visible by default

	//if is mask, find color at given coordinates in the mask (value greater than 0 = visible)
	return mask->gray_uv(u, v) > 0;
}


ostream& operator<<(ostream& os, const Sphere& sphere)
{
	cout << "Sphere: " << sphere.center << " " << sphere.radius << " ";

	return os;
}


istream& operator>>(istream& is, Sphere& sphere)
{
	string word;

	is >> word >> sphere.center;		// get center point
	is >> word >> sphere.radius;		// get radius

	sphere.readApperance(is);			// get appearance of sphere


	return is;
}