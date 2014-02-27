#ifndef SKY_CLASS
#define SKY_CLASS

#include "CommonClasses.h"
#include "Loader_Config.h"
#include "Loader_Model.h"
#include "Loader_Shader.h"

class Sky
{
public:

	Sky(std::string fileName_arg);
	~Sky();

	virtual void load() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

protected:

	GLuint VAO;
	std::string fileName,
				skyModelName;
	
	ConfigFile skyFile;
	ModelLoader::GenericModel *skyModel;
	ShaderLoader::BaseShader  *skyShader;
	Common::ObjectParameters  *skyModelParam;
};

#endif