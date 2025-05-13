
// This file contains the main class that provides drawing capabilities.
//
// Simple wrapper for a 2D image with the following functionality:
// * create a view area of given dimensions
// * set and retrieve the color of a pixel
// * save the view area as a PPM image


#ifndef IMAGE_H
#define IMAGE_H

#include "Color.h"

#include <string>

class Image
{
public:
	/**
	 * Creates a canvas of viewable area with range:
	 *     x in [0, w-1]
	 *     y in [0, h-1]
	 */
	Image(int w, int h);

	/**
	 * Frees the storage used for various buffers.
	 */
	~Image();

	/**
	* Creates a color image from a binary PPM file
	*/
	Image(string filename);

	/**
	 * Returns the color of the pixel with the given coordinates (x, y).
	 */
	Color getPixel(int x, int y) const;

	/**
	* Returns the color at the given coordinates (x, y) with interpolation.
	*/
	Color getPixel(float w, float h) const;

	/**
	* Returns the color for a given (u,v) pair. (u,v) are assumed to be in range [0..1]
	*/
	Color rgb_uv(float u, float v) const;

	/**
	* Returns the color for a given (w,h) pair. (w,h) are assumed to be exact coordinates witin an image
	*/
	Color rgb_wh(float w, float h) const;

	/**
	* Returns the gray value of the pixel at the given coordinates
	*/
	float gray_uv(float u, float v) const;

	/**
	* Returns the gray value of the pixel at the given coordinates. (w,h) are assumed to be exact coordinates in image.
	*/
	float gray_wh(float w, float h) const;

	/**
	 * Sets the pixel with the given coordinates (x, y) to the given color c.
	 */
	void setPixel(int x, int y, const Color& c) const;

	/**
	* Returns a pair (du, dv) that represents the image gradient along
	* the u, v directions, respectively (u and v are in range [0..1]).
	*/
	pair<float, float> gradient(float u, float v) const;

	/**
	* Returns a pair (du, dv) that represents the image gradient along
	* the w, h directions, respectively (w and h are exact coordinates -> not in range [0..1]).
	*/
	pair<float, float> gradient_wh(float w, float h) const;


	/**
	 * Saves a PPM image (in raw, i.e.binary format) to a file with the given name.
	 */
	void saveImage(const std::string& file_name) const;

	/**
	 * Erases the canvas. The view area is set to black and border to gray.
	 */
	void clear();

private:
	/**
	 * Fills in the given color a rectangular region anchored at (w0, h0) and
	 * extending *width* units horizontally and *height* units vertically.
	 */
	void fillRegion(int w0, int h0, int width, int height, const Color& c) const;

	/**
	 * Reserves a 2D array for the color buffer of appropriate size (view + border).
	 */
	void makeCanvas();

	/**
	 * Frees the storage used for the color buffer.
	 */
	void destroyCanvas() const;

private:
	// the canvas dimensions
	int width;
	int height;

	// the color buffer
	Color** image = nullptr;
};

#endif