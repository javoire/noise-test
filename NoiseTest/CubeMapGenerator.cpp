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

	array<Image, 6> GenerateCubeImages(int size)
	{
		array<Image, 6> images;
		Image imagePlusX;
		Image imageMinusX;
		Image imagePlusY;
		Image imageMinusY;
		Image imagePlusZ;
		Image imageMinusZ;

		auto radius = size / 2;
		int x;
		int y;
		int z;
		auto count = 0;
		auto totalIterations = size*size;

		imagePlusX.SetSize(size, size);
		imageMinusX.SetSize(size, size);
		imagePlusY.SetSize(size, size);
		imageMinusY.SetSize(size, size);
		imagePlusZ.SetSize(size, size);
		imageMinusZ.SetSize(size, size);

		// loop over 128x128 plane, uv coords
		for (auto u = 0; u < size; u++)
		{
			for (auto v = 0; v < size; v++)
			{
				//// x+ side
				x = radius;
				y = v - radius;
				z = u - radius;

				// apply value on cube face
				imagePlusX.SetValue(u, v, GetColorAtCoords(x, y, z));

				//// x- side
				x = -radius;
				y = v - radius;
				z = u - radius;

				// apply value on cube face
				imageMinusX.SetValue(u, v, GetColorAtCoords(x, y, z));

				//// y+ side
				y = radius;
				x = v - radius;
				z = u - radius;

				// apply value on cube face
				imagePlusY.SetValue(u, v, GetColorAtCoords(x, y, z));

				//// y- side
				y = -radius;
				x = v - radius;
				z = u - radius;

				// apply value on cube face
				imageMinusY.SetValue(u, v, GetColorAtCoords(x, y, z));

				//// z+ side
				z = radius;
				x = u - radius;
				y = v - radius;

				// apply value on cube face
				imagePlusZ.SetValue(u, v, GetColorAtCoords(x, y, z));

				//// z- side
				z = -radius;
				x = u - radius;
				y = v - radius;

				// apply value on cube face
				imageMinusZ.SetValue(u, v, GetColorAtCoords(x, y, z));

			}

			/*auto percent = float(count) / float(totalIterations) * 100.0f;
			cout << percent << endl;
			count++;*/
		}



		images[0] = imagePlusX;
		images[1] = imageMinusX;
		images[2] = imagePlusY;
		images[3] = imageMinusY;
		images[4] = imagePlusZ;
		images[5] = imageMinusZ;

		return images;
	}

	Color GetColorAtCoords(double x, double y, double z)
	{
		Perlin perlin;
		auto vectorCube = Vector3(x, y, z);
		auto starBrightnessReduce = 2;
		auto noiseScale = 4;
		auto starProbability = 0.001; // %
		auto noiseSoftness = 2;

		// normalize vector to get sphere surface
		auto vectorSphere = Vector3::Normalized(vectorCube);

		// get noise value at sphere surface [-1,1]
		auto noiseVal = perlin.GetValue(vectorSphere.x * noiseScale, vectorSphere.y * noiseScale, vectorSphere.z * noiseScale);

		// for some reason perlin sometimes returns a value out of bounds
		// unfortunately we're clipping the value.
		noiseVal = min(1.0, noiseVal);
		noiseVal = max(-1.0, noiseVal);

		// scale [0,1]
		noiseVal = (noiseVal + 1) / 2;

		// adjust softness. e.g scale to [0.2, 0.8]
		// scale
		noiseVal = noiseVal / noiseSoftness;
		// then "center" the noise noise range 
		noiseVal += (1 - noiseVal) / 2;

		// this pixel is a star based on probability

		auto rand = dis(gen);
		auto isStar = rand < starProbability;

		// check if star and scale to color range [0,255]
		uint8 colorValue = isStar ? (noiseVal * 255 / starBrightnessReduce) : 0;

		// get color 
		auto color = Color(colorValue, colorValue, colorValue, 1);

		return color;
	}
};
