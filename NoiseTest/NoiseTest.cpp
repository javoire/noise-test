#include <array>
#include <string>
#include "noiseutils.h"
#include "CubeMapGenerator.cpp"
using namespace std;
using namespace utils;

int main(int argc, char** argv)
{
	ImageGenerator generator;
	auto images = generator.GenerateCubeImages(1024);
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