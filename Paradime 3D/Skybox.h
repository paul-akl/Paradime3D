#ifndef SKYBOX_CLASS
#define SKYBOX_CLASS

#include "CommonClasses.h"
//#include "CurrentUpdateState.h"
#include "Loader_Model.h"
#include "Loader_Shader.h"
#include "Loader_Texture.h"

#include <vector>

class Skybox
{
public:

	Skybox(std::string fileName_arg);
	~Skybox();

	void load();
	void update();
	void render();

private:
	
	Common::ObjectParameters *objectParameters;
	TextureLoader::Texture3D *texture3D;
	ModelLoader::GenericModel *sphere;
	ShaderLoader::Shader *shader;
	std::string fileName,
				modelName,
				vertexShaderFileName,
				fragmentShaderFileName;
	GLuint		VAO;

	std::vector<TextureLoader::Texture2D*> *texturePool;

	void loadFromFile();
};

#endif