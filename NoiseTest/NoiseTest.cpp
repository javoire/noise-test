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
	NoiseMap heightMap;
	Image image;
	Color color;

	double z = 128;
	image.SetSize(256, 256);
	// loop over 128x128 plane, xyz coords
	for (int x = -128; x < 128; x++) {
		for (int y = 128; y > -128; y--)
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
			double dValue = perlin.GetValue((x / dLength*10), (y / dLength*10), (z / dLength*10));

			/*double dUnitLength = sqrt(pow(x / dLength, 2)
				+ pow(y / dLength, 2)
				+ pow(z / dLength, 2));
				*/

			uint8 value = (uint8)((uint)(dValue * 255.0) & 0xff);

			Color color(value, value, value, 1);

			// set value on image
			image.SetValue(x+128, abs(y-128), color);
		}
	}

	// write image

	/*
	utils::NoiseMapBuilderSphere heightMapBuilder;
	heightMapBuilder.SetSourceModule(perlin);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(512, 256);
	heightMapBuilder.SetBounds(-90.0, 90.0, -180.0, 180.0);
	heightMapBuilder.Build();

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.Render();
	*/

	WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename("test.bmp");
	writer.WriteDestFile();

	return 0;
}
