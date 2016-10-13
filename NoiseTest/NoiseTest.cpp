#include "stdafx.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <noise/noise.h>
#include <array>
#include <string>
#include "noiseutils.h"
using namespace std;
using namespace noise;
using namespace module;
using namespace utils;

class Vector3
{
public:
	double x;
	double y;
	double z;

	Vector3(double _x, double _y, double _z) {
		x = _x;
		y = _y;
		z = _z;
	};

	double GetLength() {
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	void Normalize() {
		double length = GetLength();
		x = x / length;
		y = y / length;
		z = z / length;
	}

	static Vector3 Normalized(Vector3 v) {
		Vector3 vector = Vector3(v.x, v.y, v.z);
		double length = vector.GetLength();
		vector.x = vector.x / length;
		vector.y = vector.y / length;
		vector.z = vector.z / length;
		return vector;
	}
};

class ImageGenerator
{
public:
	static array<Image, 6> GenerateCubeImages() {
		array<Image, 6> images;
		Image imagePlusX;
		Image imageMinusX;
		Image imagePlusY;
		Image imageMinusY;
		Image imagePlusZ;
		Image imageMinusZ;

		int size = 128;
		int radius = size / 2;
		int x;
		int y;
		int z;

		imagePlusX.SetSize(size, size);
		imageMinusX.SetSize(size, size);
		imagePlusY.SetSize(size, size);
		imageMinusY.SetSize(size, size);
		imagePlusZ.SetSize(size, size);
		imageMinusZ.SetSize(size, size);

		// loop over 128x128 plane, xyz coords
		for (int u = 0; u < size; u++)
		{
			for (int v = 0; v < size; v++)
			{
				//// x+ side
				x = radius;
				y = v + radius;
				z = u - radius;

				// apply value on cube face
				imagePlusX.SetValue(u, v, GetColorAtCoords(x, y, z));

				//// x- side
				x = -radius;
				y = v + radius;
				z = u + radius;

				// apply value on cube face
				imagePlusX.SetValue(u, v, GetColorAtCoords(x, y, z));

				//// y+ side
				y = radius;
				x = v + radius;
				z = u - radius;

				// apply value on cube face
				imagePlusX.SetValue(u, v, GetColorAtCoords(x, y, z));

				//// y- side
				y = -radius;
				x = v + radius;
				z = u - radius;

				// apply value on cube face
				imagePlusX.SetValue(u, v, GetColorAtCoords(x, y, z));

				//// z+ side
				z = radius;
				x = u + radius;
				y = v + radius;

				// apply value on cube face
				imagePlusX.SetValue(u, v, GetColorAtCoords(x, y, z));

				//// z- side
				z = -radius;
				x = u - radius;
				y = v + radius;

				// apply value on cube face
				imagePlusX.SetValue(u, v, GetColorAtCoords(x, y, z));
			}
		}

		images[0] = imagePlusX;
		images[1] = imageMinusX;
		images[2] = imagePlusY;
		images[3] = imageMinusX;
		images[4] = imagePlusZ;
		images[5] = imageMinusZ;

		int numbers[3];

		return images;
	}

	static Color GetColorAtCoords(double x, double y, double z) {
		Perlin perlin;
		Vector3 vectorCube = Vector3(x, y, z);

		// normalize vector to get sphere surface
		Vector3 vectorSphere = Vector3::Normalized(vectorCube);

		// get noise value at sphere surface [-1,1]
		double noiseVal = perlin.GetValue(vectorSphere.x, vectorSphere.y, vectorSphere.z);

		// scale [0,1]
		noiseVal = (noiseVal + 1) / 2;

		// scale to color range [0,255]
		uint8 colorValue = (noiseVal * 255);

		// get color 
		Color color = Color(colorValue, colorValue, colorValue, 1);

		return color;
	}
};

int main(int argc, char** argv)
{
	array<Image, 6> images = ImageGenerator::GenerateCubeImages();
	WriterBMP writer;

	int count = 0;
	for (Image image : images) {
		writer.SetSourceImage(image);
		writer.SetDestFilename("image" + to_string(count) + ".bmp");
		writer.WriteDestFile();
		count++;
	}


	return 0;
}