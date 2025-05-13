#include "Image.h"
#include "Color.h"

#include <algorithm>
#include <string>
#include <ostream>
#include <fstream>

using uchar = unsigned char;


void Image::setPixel(int w, int h, const Color& c) const
{
	image[h][w] = c;
}


Color Image::getPixel(int w, int h) const
{	
	int wrapH = (h % height + height) % height;						//handle wrapping around in both directions
	int wrapW = (w % width + width) % width;							
	return image[wrapH][wrapW];											
}


Color Image::getPixel(float w, float h) const
{
	//interpolate non integer coordinates to get smoother color changes in renderings
	float i0 = w - floor(w);
	float i1 = 1 - i0;
	float j0 = h - floor(h);
	float j1 = 1 - j0;

	//coordinates for finding neighbor pixels (x & y be default represent LL pixel)
	int x = floor(w);
	int y = floor(h);

	Color c1 = (j0 * getPixel(x, y+1)) + (j1 * getPixel(x, y));
	Color c2 = (j0 * getPixel(x+1, y+1)) + (j1 * getPixel(x+1, y));


	return (i0 * c2) + (i1 * c1);			// return interpolated color	
}


Color Image::rgb_uv(float u, float v) const
{
	float i = u * width;
	float j = v * height;

	return rgb_wh(i, j);		// Set color of P(x,y,z) on shape to color at image(i,j);
}


Color Image::rgb_wh(float w, float h) const
{
	return getPixel(w, h);     // Set color of P(x,y,z) on shape to color at image(w,h);
}


float Image::gray_uv(float u, float v) const
{
	Color c = this->rgb_uv(u, v);

	return gray(c);
}


float Image::gray_wh(float w, float h) const
{
	Color c = this->rgb_wh(w, h);

	return gray(c);
}


pair<float, float> Image::gradient(float u, float v) const
{
	//u, v are in range [0..1] -> represent some percentage of image width, and height respectively
	float incrU = 1.0 / width;												// get next pixel width-wise
	float incrV = 1.0 / height;
	float du = (gray_uv(u + incrU, v) - gray_uv(u - incrU, v)) / 2;			// both in range [-1..1]
	float dv = (gray_uv(u, v + incrV) - gray_uv(u, v - incrV)) / 2;

	return pair<float, float> {du, dv};
}

pair<float, float> Image::gradient_wh(float w, float h) const
{
	//w, h are exact coordinates, so only need to add one to go to next pixel for width/height
	float du = (gray_wh(w + 1, h) - gray_wh(w - 1, h)) / 2;			// both in range [-1..1]
	float dv = (gray_wh(w, h + 1) - gray_wh(w, h - 1)) / 2;

	return pair<float, float> {du, dv};
}


Image::Image(int width, int height)
	:
	width(width),
	height(height)
{
	makeCanvas();
	clear();
}


Image::~Image()
{
	destroyCanvas();
}


Image::Image(string filename)
{
	std::ifstream ifs(filename, std::ios::binary);

	string token;
	float channelMax;

	// read header info
	ifs >> token;				// type of PPM file
	ifs >> width >> height;		// width, height of image
	ifs >> channelMax;			// max value per channel


	//make the 'canvas' before writing to it
	makeCanvas();


	// read rgb info
	ifs.ignore(1024, '\n');		// will remove the '\n' after 255
	ifs >> noskipws;			// will ensure spaces are not skipped

	uchar r;
	uchar g;
	uchar b;
	Color color;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			ifs >> r >> g >> b;								// read in triples for each rgb value

			float rComp = (int)r / channelMax;
			float gComp = (int)g / channelMax;
			float bComp = (int)b / channelMax;
			color = Color(rComp, gComp, bComp);				// convert from [0 to channelMax] into [0 to 1] and create color

			setPixel(x, y, color);							// set this pixel in the image	
		}
	}
}


void Image::clear()
{
	//set the whole canvas to black
	fillRegion(0, 0, width, height, BLACK);
}


void Image::saveImage(const std::string& file_name) const
{
	std::ofstream ofs(file_name.c_str(), std::ios::binary);

	// write the header information
	ofs << "P6" << std::endl;
	ofs << width << "\t" << height << std::endl;
	ofs << 255 << std::endl;

	// save the image data one row at a time
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Color c = getPixel(x, y);
			ofs << uchar(255 * c.r()) << uchar(255 * c.g()) << uchar(255 * c.b());
		}
	}
}


void Image::fillRegion(int w0, int h0, int width, int height, const Color& c) const
{
	int w1 = w0 + width;
	int h1 = h0 + height;

	for (int y = h0; y < h1; ++y) {
		for (int x = w0; x < w1; ++x) {
			setPixel(x, y, c);
		}
	}
}


void Image::makeCanvas()
{
	image = new Color * [height];
	for (int h = 0; h < height; ++h) {
		image[h] = new Color[width];
	}
}


void Image::destroyCanvas() const
{
	for (int h = 0; h < height; ++h) {
		delete[] image[h];
	}
	delete[] image;
}