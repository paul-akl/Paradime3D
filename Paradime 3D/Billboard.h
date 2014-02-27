#ifndef BILLBOARD_CLASS
#define BILLBOARD_CLASS

#include "Loader_Shader.h"
#include "Loader_Texture.h"

class Billboard
{
public:
	Billboard(std::string geometryShaderName_arg, std::string vertexShaderName_arg, std::string fragmentShaderName_arg, std::string texture_arg);
	~Billboard();

	void render();

	void setTexture(std::string textureString_arg);
	void setTexture(TextureLoader::Texture2D *texture_arg);

	void setScale(Math3d::Vec3f scale_arg);
	void setPosition(Math3d::Vec3f position_arg);
	void setRotation(Math3d::Vec3f rotation_arg);

	bool enabled;
	Common::ObjectParameters *parameters;

private:

	ShaderLoader::GeomShader *shader;
	TextureLoader::Texture2D *texture;

	GLuint VAO, buffer;

};

#endif