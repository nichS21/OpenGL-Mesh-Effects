#include "Material.h"
#include <string>
using namespace std;

void Material::load(const Color& c) const
{
	GLfloat mat_amb[] = { c.r() * ka, c.g() * ka, c.b() * ka, 1 };	//ka*color
	GLfloat mat_diff[] = { c.r() * kd, c.g() * kd, c.b() * kd, 1 };	//kd*color
	GLfloat mat_spec[] = { c.r() * ks, c.g() * ks, c.b() * ks, 1 };	//ks*color
	GLfloat mat_phong[] = { n };									//0 to 128 only


	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_phong);
}

// Display material in format: 'Mat(ka, kd, ks, n)'
ostream& operator<<(ostream& os, const Material& mat) 
{
	cout << "Mat(" << mat.ka << "," << mat.kd << "," << mat.ks << "," << mat.n << ")";
	
	return os;
}

//
istream& operator>>(istream& is, Material& mat)
{
	string word;

	is >> word >> mat.ka;	// set ambient coeff
	is >> word >> mat.kd;	// etc..
	is >> word >> mat.ks;
	is >> word >> mat.n;	// set Phong size 

	is >> word;			// consume 'end'

	return is;
}