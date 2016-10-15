#include <array>
#include <string>
#include "noiseutils.h"
#include "CubeMapGenerator.cpp"
using namespace std;
using namespace utils;

int main(int argc, char** argv)
{
	array<Image, 6> images = ImageGenerator::GenerateCubeImages();
	WriterBMP writer;

	auto count = 0;
	for (auto image : images) {
		writer.SetSourceImage(image);
		writer.SetDestFilename("image" + to_string(count) + ".bmp");
		writer.WriteDestFile();
		count++;
	}

	return 0;
}