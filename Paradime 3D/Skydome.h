#ifndef SKYSPHERE_CLASS
#define SKYSPHERE_CLASS

#include "CommonClasses.h"
#include "Loader_Model.h"
#include "Loader_Shader.h"
#include "Loader_Texture.h"
#include "Sky.h"

#include <vector>

class Skydome : public Sky
{
public:

	Skydome(std::string fileName_arg);
	~Skydome();

	void load();
	void update();
	void render();

private:
	
	TextureLoader::Texture2D *glowMap, *colorMap, *sun, *moon;
	std::string glowMapName,
				colorMapName,
				sunImageName,
				moonImageName,
				geometryShaderFileName,
				vertexShaderFileName,
				fragmentShaderFileName;
		
	Billboard *sunObject, *moonObject;
	Math3d::Vec3f skyObjectsOffset, skydomeOffset,
				  sunOffset, moonOffset;

	void loadFromFile();
};

#endif