#include "Sky.h"

Sky::Sky(std::string fileName_arg)
{
	fileName = fileName_arg;
	VAO = 0;
}
Sky::~Sky()
{
	delete skyModel;
	delete skyShader;
	delete skyModelParam;
}