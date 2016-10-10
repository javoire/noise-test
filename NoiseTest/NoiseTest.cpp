// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <noise/noise.h>
#include "noiseutils.h"
using namespace std;
using namespace noise;

int main(int argc, char** argv)
{
	module::Perlin myModule;
	utils::NoiseMap heightMap;

	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(myModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(256, 256);
	heightMapBuilder.SetBounds(0.0,5.0, 0.0, 5.0);
	heightMapBuilder.Build();

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);

	return 0;
}
