#include "Billboard.h"
#include "CurrentUpdateState.h"

Billboard::Billboard(std::string geometryShaderName_arg, std::string vertexShaderName_arg, std::string fragmentShaderName_arg, std::string texture_arg)
{
	// Create and bind the Vertex Array Object
	glGenVertexArrays(1, &VAO);   
	glBindVertexArray(VAO);

	shader = ShaderLoader::load(geometryShaderName_arg, vertexShaderName_arg, fragmentShaderName_arg);
	texture = TextureLoader::load2D(texture_arg);

	Math3d::Vec3f position(0.0f);

    glGenBuffers(1, &buffer);
  	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), &position, GL_STATIC_DRAW);

	parameters = new Common::ObjectParameters();
}
Billboard::~Billboard()
{
	delete parameters;
}

void Billboard::render()
{
	if(enabled)
	{
		glBindVertexArray(VAO);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);

		Current::objectParameters = parameters;
	
		shader->bind();
		texture->bind();
	
		shader->updateFrame();
		shader->updateModel();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_POINTS, 0, 1);
    
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		glDepthFunc(Config::engine::depth_test_func);
	}
}

void Billboard::setTexture(std::string textureString_arg)
{
	texture = TextureLoader::load2D(textureString_arg);
}
void Billboard::setTexture(TextureLoader::Texture2D *texture_arg)
{
	texture = texture_arg;
}

void Billboard::setScale(Math3d::Vec3f scale_arg)
{
	parameters->scaleVec = scale_arg;
	parameters->modelMat.scale(parameters->scaleVec);
}
void Billboard::setPosition(Math3d::Vec3f position_arg)
{
	parameters->positionVec = position_arg;
	parameters->modelMat.transform(parameters->positionVec);
}
void Billboard::setRotation(Math3d::Vec3f rotation_arg)
{
	parameters->rotationVec = rotation_arg;
	parameters->modelMat.rotate(parameters->rotationVec);
}