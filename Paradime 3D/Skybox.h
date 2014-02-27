#ifndef SKYBOX_CLASS
#define SKYBOX_CLASS

#include "CommonClasses.h"
//#include "CurrentUpdateState.h"
#include "Loader_Model.h"
#include "Loader_Shader.h"
#include "Loader_Texture.h"
#include "Sky.h"

#include <vector>

class Skybox : public Sky
{
public:

	Skybox(std::string fileName_arg);
	~Skybox();

	void load();
	void update();
	void render();

private:
	
	TextureLoader::Texture3D *skyCubeMap;
	std::string vertexShaderFileName,
				fragmentShaderFileName;

	std::vector<TextureLoader::Texture2D*> *texturePool;

	void loadFromFile();
};

#endif