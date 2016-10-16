#include <array>
#include <algorithm>
#include "noiseutils.h"
#include "Vector3.cpp"
#include <iostream>
#include <random>

#define _USE_MATH_DEFINES

using namespace std;
using namespace utils;
using namespace noise;
using namespace module;

class ImageGenerator
{
public:

	std::mt19937 gen;
	uniform_real_distribution<> dis;
	
	ImageGenerator()
	{
		std::random_device rd;
		gen = std::mt19937(rd());
		dis = uniform_real_distribution<>(0, 1);
	}

	// I used this to figure out the correct rotations of the images (cube faces)
	Image GenerateTestImage(int size) const
	{
		Image testImage;
		testImage.SetSize(size*6, size);

		for (auto v = 0; v < size; v++)
		{
			for (auto u = 0; u < size; u++)
			{
				Color color;
				if (v > (size/4*3))
				{
					color = Color(uint8(0), uint8(0), uint8(100), 1);
				}
				else
				{
					color = Color(uint8(100), uint8(100), uint8(100), 1);
				}
				testImage.SetValue(v, u, color); // rotate -90
				testImage.SetValue((size - v - 1) + size, size - u - 1, color); // rotate +90
				testImage.SetValue(u + size * 2, v, color);
				testImage.SetValue(u + size * 3, size - v - 1, color); // rotate 180
				testImage.SetValue(u + size * 4, size - v - 1, color); // rotate 180
				testImage.SetValue(u + size * 5, size - v - 1, color); // rotate 180
			}
		}

		return testImage;
	}

	Image GenerateCubeImages(int size)
	{
		Image cubeMapImage;
		auto radius = size / 2;
		auto count = 0;
		auto totalIterations = size*size;
		
		// offset each image by n*size
		int offset;

		cubeMapImage.SetSize(size*6, size);

		// loop over 128x128 plane, uv coords
		for (auto s = 0; s < size; s++)
		{
			for (auto t = 0; t < size; t++)
			{

				// st (uv) mapping according to
				// major axis
				// direction     target                              sc     tc    ma
				// ----------    -------------------------------- - -- - -- - -- -
				// +rx          GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT   -rz	-ry	  rx
				// -rx          GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT   +rz	-ry	  rx
				// +ry          GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT   +rx	+rz	  ry
				// -ry          GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT   +rx	-rz	  ry
				// +rz          GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT   +rx	-ry	  rz
				// -rz          GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT   -rx	-ry	  rz
				//
				// http://www.nvidia.com/object/cube_map_ogl_tutorial.html
				//
				// but for some reason, the -x and +x sides have to switch places...

				//// x- side
				cubeMapImage.SetValue(s, t, GetColorAtCoords(-radius, t - radius, radius - s));

				//// x+ side
				offset = size;
				cubeMapImage.SetValue(s + offset, t, GetColorAtCoords(radius, t - radius, s - radius));

				//// y+ side
				offset = size * 2;
				cubeMapImage.SetValue(s + offset, t, GetColorAtCoords(radius - s, radius, radius - t));

				//// y- side
				offset = size * 3;
				cubeMapImage.SetValue(s + offset, t, GetColorAtCoords(radius - s, -radius, t - radius));

				//// z+ side
				offset = size * 4;
				cubeMapImage.SetValue(s + offset, t, GetColorAtCoords(radius - s, t - radius, radius));

				//// z- side
				offset = size * 5;
				cubeMapImage.SetValue(s + offset, t, GetColorAtCoords(s - radius, t - radius, -radius));
			}
			/*auto percent = float(count) / float(totalIterations) * 100.0f;
			cout << percent << endl;
			count++;*/
		}

		return cubeMapImage;
	}

	Color GetColorAtCoords(double x, double y, double z)
	{
		Perlin perlin;
		auto vectorCube = Vector3(x, y, z);
		auto starBrightnessReduce = 1;
		auto noiseScale = 1;
		auto starProbability = 0.001; // %
		auto noiseSoftness = 1;

		// normalize vector to get sphere surface
		auto vectorSphere = Vector3::Normalized(vectorCube);

		// get noise value at sphere surface [-1,1]
		auto noiseVal = perlin.GetValue(vectorSphere.x * noiseScale, vectorSphere.y * noiseScale, vectorSphere.z * noiseScale);

		// for some reason perlin sometimes returns a value out of bounds
		// (unfortunately we're clipping the value here)
		noiseVal = min(1.0, noiseVal);
		noiseVal = max(-1.0, noiseVal);

		// scale [0,1]
		noiseVal = (noiseVal + 1) / 2;

		// threshold debug
		if (noiseVal > 0.7)
		{
			noiseVal = 1;
		} else if (noiseVal <= 0.3) 
		{
			noiseVal = 0;
		}

		// adjust softness. e.g scale to [0, 0.5]
		// scale
		noiseVal = noiseVal / noiseSoftness;
		// then "center" the noise noise range 
		//noiseVal += noiseSoftness / 2;

		// this pixel is a star based on probability
		//auto rand = dis(gen);
		//auto isStar = rand < starProbability;
		auto isStar = true;

		// check if star and scale to color range [0,255]
		uint8 colorValue = isStar ? (noiseVal * 255 / starBrightnessReduce) : 0;

		// get color 
		auto color = Color(colorValue, colorValue, colorValue, 1);

		// debug lines
		if (abs(vectorSphere.x) > abs(vectorSphere.y)
			&& vectorSphere.y > 0
			&& vectorSphere.z < 0.1
			&& vectorSphere.z > -0.1) // x
		{
			auto strength = x > 0 ? 230 : 60; // positive x is brighter
			color = Color(uint8(strength), 0, 0, 1);
		}
		if (abs(vectorSphere.y) > abs(vectorSphere.x)
			&& vectorSphere.x > 0
			&& vectorSphere.z < 0.1
			&& vectorSphere.z > -0.1) // y
		{
			auto strength = y > 0 ? 230 : 60;
			color = Color(0, uint8(strength), 0, 1);
		}
		if (abs(vectorSphere.z) > abs(vectorSphere.y)
			&& vectorSphere.y > 0
			&& vectorSphere.x < 0.1
			&& vectorSphere.x > -0.1) // z
		{
			auto strength = z > 0 ? 230 : 60;
			color = Color(0, 0, uint8(strength), 1);
		}
		return color;
	}
};
