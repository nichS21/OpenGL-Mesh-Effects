#include "Color.h"

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// given only to show how to call another constructor
//
// Color::Color()
//   :
//   Color(0, 0, 0)
// {
// }

Color::Color(float r, float g, float b)
	:
	r_(r), g_(g), b_(b)
{
	assert(r_ >= 0);
	assert(g_ >= 0);
	assert(b_ >= 0);

	r_ = (r_ > 1) ? 1 : r_;
	g_ = (g_ > 1) ? 1 : g_;
	b_ = (b_ > 1) ? 1 : b_;
}

float Color::r() const
{
	return r_;
}

float Color::g() const
{
	return g_;
}

float Color::b() const
{
	return b_;
}

Color Color::operator+(const Color& c2) const
{
	//float r = r_        + c2.r();
	//float r = r()       + c2.r();
	//float r = this->r() + c2.r();

	float r = r_ + c2.r();
	float g = g_ + c2.g();
	float b = b_ + c2.b();

	Color color(r, g, b);
	return color;
}

Color operator-(const Color& c1, const Color& c2)
{
	float r = c1.r() - c2.r();
	float g = c1.g() - c2.g();
	float b = c1.b() - c2.b();

	Color color(r, g, b);
	return color;
}

Color Color::operator*(float alpha) const
{
	float r = r_ * alpha;
	float g = g_ * alpha;
	float b = b_ * alpha;

	Color color(r, g, b);
	return color;
}

Color operator*(float alpha, const Color& color)
{
	// alpha*color == color*alpha; reusing the method version
	return color * alpha;
}

Color operator*(const Color& c1, const Color& c2)
{
	float r = c1.r() * c2.r();
	float g = c1.g() * c2.g();
	float b = c1.b() * c2.b();

	Color color(r, g, b);
	return color;
}

float gray(const Color& c)
{
	return (0.299*c.r()) + (0.587*c.g()) + (0.114*c.b());
}

// "friend" implementation of the input/output operators
// have access to the private data members/methods

ostream& operator<<(ostream& os, const Color& color)
{
	// os << format("Color({}, {}, {})", color.r(), color.g(), color.b());

	// could use this, since members accessible:
	//     os << "Color(" << color.r_ << ", " << color.g_ << ", " << color.b_ << ")";

	// chose to use this, since have selectors:
	os << "Color(" << color.r() << ", " << color.g() << ", " << color.b() << ")";

	return os;
}

istream& operator>>(istream& is, Color& color)
{
	// format is:          rgb <1, 1, 1>

	string tag;
	char ch;

	is >> tag;             // read the word   rgb
	is >> ch;              // read <
	is >> color.r_ >> ch;  // read color and comma
	is >> color.g_ >> ch;
	is >> color.b_;        // no comma, just color
	is >> ch;              // read >

	return is;
}

//    "non-friend" implementation of the input/output operators

//    ostream& operator<<(ostream& os, const Color& color)
//    {
//     	// os << format("Color({}, {}, {})", color.r(), color.g(), color.b());
//    	os << "Color(" << color.r() << ", " << color.g() << ", " << color.b() << ")";
//    	return os;
//    }

//    istream& operator>>(istream& is, Color& color)
//    {
//    	// format is:      rgb <1, 1, 1>

//    	string tag;
//    	char ch;
//    	float r, g, b;

//    	is >> tag;      // read the word   rgb
//    	is >> ch;       // read <
//    	is >> r >> ch;  // read color and comma
//    	is >> g >> ch;
//    	is >> b;        // no comma, just color
//    	is >> ch;       // read >

//    	color = Color(r, g, b);   // make the color

//    	return is;
//    }