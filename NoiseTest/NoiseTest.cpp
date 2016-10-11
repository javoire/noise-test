// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <noise/noise.h>
#include "noiseutils.h"
using namespace std;
using namespace noise;
using namespace module;
using namespace utils;

class ImageGenerator
{
public:
	static Image RenderOneImage() {
		Perlin perlin;
		Image image;

		double z = 128;
		image.SetSize(256, 256);
		// loop over 128x128 plane, xyz coords
		for (double x = -128; x < 128; x++) {
			for (double y = 128; y > -128; y--)
			{
				// get real length
				double powX = pow(x, 2);
				double powY = pow(y, 2);
				double powZ = pow(z, 2);
				double dLength = abs(sqrt(powX + powY + powZ));

				// get unit length (as if on a sphere)
				//double dUnitLength = sqrt(pow(x / dLength, 2)
				//+ pow(y / dLength, 2)
				//+ pow(z / dLength, 2));

				// get noise for unit vector (as if on sphere)
				double dScale = 0.25;
				double dValue = perlin.GetValue(((x / dScale) / dLength), ((y / dScale) / dLength), ((z / dScale) / dLength));

				/*double dUnitLength = sqrt(pow(x / dLength, 2)
				+ pow(y / dLength, 2)
				+ pow(z / dLength, 2));
				*/

				double dValueNormalized = (dValue + 1) / 2;

				// for some reason it flips if it goes to close to 0 or 1...
				if (dValueNormalized > 0.999)
				{
					dValueNormalized = 0.999;
				}
				if (dValueNormalized < 0.001)
				{
					dValueNormalized = 0.001;
				}

				uint8 value = (uint8)(dValueNormalized * 255.0);

				Color color(value, value, value, 1);

				// set value on image
				image.SetValue(x + 128, abs(y - 128), color);

				return image;
			}
		}
	}

	static Image RenderSixImages() {

		Perlin perlin;
		// create 6 empty images
		// xPlus xMins yPlus yMinus zPlus zMinus
		Image imageXplus;
		Image imageXminus;
		Image imageYplus;
		Image imageYminus;
		Image imageZplus;
		Image imageZminus;

		imageXplus.SetSize(90, 90);

		// debug
		int countX = 0;
		int countY = 0;
		int countZ = 0;

		// loop over lat and lon, 0-360 0-360
		for (int lat = 0; lat < 360; lat++)
		{
			for (int lon = 0; lon < 360; lon++)
			{
				double latRad = lat * (M_PI / 180);
				double lonRad = lon * (M_PI / 180);
				
				// get x,y,z value for position
				// i.e. convert polar to radial
				// radius is implied to be 1 here:
				double x = cos(latRad);
				double y = sin(latRad);
				double z = sin(lonRad);

				double xAbs = abs(x);
				double yAbs = abs(y);
				double zAbs = abs(z);

				// pick perlin noise value 
				double noiseValue = perlin.GetValue(x, y, z);
				uint8 uValue = (uint8)(abs((noiseValue+1)/2) * 255.0);

				// figure out the coordinate on the image to set the value to

				// figure out which plane / image we're pointing at
				// i.e. which image to set a pixel color value to
				if (xAbs > yAbs && xAbs >= zAbs) {
					// x direction
					countX++;

					if (x >= 0) {
						// xPlus direction

						imageXplus.SetValue(lat, lon, Color(uValue, uValue, uValue, 1));
					}
					else {
						// xMinus direction
					}
				}
				else if (yAbs > xAbs && yAbs >= zAbs) {
					// y direction
					countY++;

					if (y >= 0) {
						// yPlus direction
						//cout << "lol";
					}
					else {
						// yMinus direction
					}
				}
				else if (zAbs > xAbs && zAbs >= yAbs) {
					// z direction
					countZ++;

					if (z >= 0) {
						// zPlus direction
						//cout << "lol";
					}
					else {
						// zMinus direction
					}
				}
			}
		}

		double totalCount = countX + countY + countZ;
		return imageXplus;
	}
};

int main(int argc, char** argv)
{

	//Image image = ImageGenerator::RenderOneImage();
	Image image = ImageGenerator::RenderSixImages();

	// write image
	WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename("test2.bmp");
	writer.WriteDestFile();

	return 0;
}