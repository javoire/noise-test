// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <noise/noise.h>
#include "noiseutils.h"
using namespace std;
using namespace noise;
using namespace module;
using namespace utils;

int main(int argc, char** argv)
{

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
			image.SetValue(x+128, abs(y-128), color);
		}
	}

	// write image
	WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename("test.bmp");
	writer.WriteDestFile();

	return 0;
}
