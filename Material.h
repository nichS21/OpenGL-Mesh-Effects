#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include "Color.h"
#include <GL/glew.h>
#include <GL/gl.h>
using namespace std;

struct Material 
{
	//Data members
	float ka;			// ambient coeffiecient
	float kd;			// diffuse coeffiecient
	float ks;			// specular coefficient
	float n;			// specular spot size (Phong size)

	//overload << as a friend method
	friend ostream& operator<<(ostream& os, const Material& mat);
	
	//load material properties for lighting purposes, given a color (openGL)
	void load(const Color& c) const;
};

//overload >> as a function
istream& operator>>(istream& is, Material& mat);


#endif