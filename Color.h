#ifndef COLOR_H
#define COLOR_H

#include <iostream>
using namespace std;

class Color
{
private:
	float r_ = 0;
	float g_ = 0;
	float b_ = 0;

public:
	Color() = default;

	Color(float r, float g, float b);

	float r() const;
	float g() const;
	float b() const;

	// overloaded binary + as method (c1 + c2)
	Color operator+(const Color& c2) const;

	// overloaded binary * as method (c1 * alpha)
	Color operator*(float alpha) const;

	// "friend" declaration of the input/output operators
	// grants access to the private data members/methods

	// display operator as function (cout << color)
	friend ostream& operator<<(ostream& os, const Color& color);

	// input operator as function (cin >> color)
	// color must be entered in this format:   rgb <1, 1, 1>
	friend istream& operator>>(istream& is, Color& color);
};

//overloaded binary - as function (c1 - c2)
Color operator-(const Color& c1, const Color& c2);

//overloaded binary * as function (alpha * color)
Color operator*(float alpha, const Color& color);

//overloaded binary * as function (color * color)
Color operator*(const Color& c1, const Color& c2);

// Returns gray value of color based on its rgb components
float gray(const Color& c);


// "non-friend" declaration of the input/output operators
//
//   // display operator as function (cout << color)
//   ostream& operator<<(ostream& os, const Color& color);

//   // input operator as function (cin >> color)
//   // color must be entered in this format:   rgb <1, 1, 1>
//   istream& operator>>(istream& is, Color& color);

const Color BLACK(0, 0, 0);
const Color WHITE(1, 1, 1);

const Color RED(1, 0, 0);
const Color GREEN(0, 1, 0);
const Color BLUE(0, 0, 1);

const Color YELLOW(1, 1, 0);
const Color MAGENTA(1, 0, 1);
const Color CYAN(0, 1, 1);

#endif /* COLOR_H */