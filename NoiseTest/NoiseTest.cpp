#include <array>
#include <string>
#include <sstream>
#include "noiseutils.h"
#include "CubeMapGenerator.cpp"
#include <ctime>
using namespace std;
using namespace utils;

int main(int argc, char** argv)
{
	ImageGenerator generator;
	auto image = generator.GenerateCubeImages(1024);
	//auto testImage = generator.GenerateTestImage(64);
	WriterBMP writer;
	
	time_t seconds;
	time(&seconds);
	std::stringstream ss;
	ss << seconds;
	std::string ts = ss.str();

	writer.SetSourceImage(image);
	writer.SetDestFilename("cubemaps/cubemap_" + ts + ".bmp");
	writer.WriteDestFile();

	//writer.SetSourceImage(testImage);
	//writer.SetDestFilename("testcubemap.bmp");
	//writer.WriteDestFile();

	return 0;
}