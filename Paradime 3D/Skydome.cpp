#include <fstream>
#include <iostream>

#include "Config.h"
#include "CurrentUpdateState.h"
#include "ErrorHandler.h"
#include "GameObjects.h"

#include "Skydome.h"

Skydome::Skydome(std::string fileName_arg) : Sky(fileName_arg)
{
	skyObjectsOffset;// = Math3d::Vec3f(-50.0f, -50.0f, -50.0f);
	skydomeOffset;// = Math3d::Vec3f(0.0f, -0.1f, 0.0f);
	Message::show(MSG_INFO, MSG_SKY, "Handle has been created.");
}
Skydome::~Skydome()
{
	delete glowMap;
	delete colorMap;
	delete sun;
	delete moon;
	delete sunObject;
}

void Skydome::load()
{
	glGenVertexArrays(1, &VAO);   
	glBindVertexArray(VAO);

	try
	{
		loadFromFile();
		
		glowMap = TextureLoader::load2D(glowMapName);
		colorMap = TextureLoader::load2D(colorMapName);
		skyShader = ShaderLoader::load(vertexShaderFileName, fragmentShaderFileName);
		
		skyModelParam = new Common::ObjectParameters();
		skyModel = new ModelLoader::GenericModel(skyModelName);
	}
	catch(Message::messageCode error)
	{
		error.display();
	}

	skyModelParam->rotationVec = Math3d::Vec3f(0, 0, 0);
	skyModelParam->modified = true;

	sunOffset = -skyObjectsOffset;
	moonOffset = skyObjectsOffset;

	glBindVertexArray(0);

	glowMap->setWrap(TextureLoader::ClampToEdge);
	colorMap->setWrap(TextureLoader::MirroredRepeat);
	
	sunObject = new Billboard("billboard.pgs", "billboard.pvs", "billboard.pfs", sunImageName);
	moonObject = new Billboard("billboard.pgs", "billboard.pvs", "billboard.pfs", moonImageName);
	//stars = new Billboard("billboard.pgs", "billboard.pvs", "billboard.pfs", "sun.png");
}
void Skydome::update()
{
	skyModelParam->modified = true;
	skyModelParam->positionVec = Current::player->currentCamera->position;
	skyModelParam->positionVec += skydomeOffset;

	Math3d::Vec3f skyobjectPosition = sunOffset;
	skyobjectPosition.rotate(Current::scene->lighting->directionalLight->direction.z, Math3d::Vec3f(0.0, 0.0, 1.0));
	skyobjectPosition.rotate(Current::scene->lighting->directionalLight->direction.x, Math3d::Vec3f(1.0, 0.0, 0.0));
	skyobjectPosition.rotate(Current::scene->lighting->directionalLight->direction.y, Math3d::Vec3f(0.0, 1.0, 0.0));
	sunObject->setPosition(skyobjectPosition + Current::scene->player.currentCamera->position);
	
	skyobjectPosition = moonOffset;
	skyobjectPosition.rotate(Current::scene->lighting->directionalLight->direction.z, Math3d::Vec3f(0.0, 0.0, 1.0));
	skyobjectPosition.rotate(Current::scene->lighting->directionalLight->direction.x, Math3d::Vec3f(1.0, 0.0, 0.0));
	skyobjectPosition.rotate(Current::scene->lighting->directionalLight->direction.y, Math3d::Vec3f(0.0, 1.0, 0.0));
	moonObject->setPosition(skyobjectPosition + Current::scene->player.currentCamera->position);

}
void Skydome::render()
{
	glBindVertexArray(VAO);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Current::objectParameters = skyModelParam;

	glowMap->bind(0);
	colorMap->bind(1);

	Current::sunGlowMap = 0;
	Current::skyMap = 1;

	skyShader->updateTextureUniforms();

	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
	
	skyShader->updateFrame();
	skyModelParam->rotationVec = Math3d::Vec3f(0.0f, 0.0f, 0.0f);
	skyModel->render(skyShader);

	glCullFace(Config::engine::face_culling_mode);
    glDepthFunc(Config::engine::depth_test_func);
	
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	
	sunObject->render();
	moonObject->render();
}

void Skydome::loadFromFile()
{
	skyFile.import(fileName);

	ConfigFile::Node *tempNode, *valueNode,
					 *rootNode = skyFile.getRootNode();

	tempNode = rootNode->getNode("Model Name");
	if(tempNode != NULL)
	{
		skyModelName = tempNode->value->getString();
	}
	else
	{
		throw Message::messageCode(MSG_ERROR, MSG_SKY, "No model found.");
	}

	tempNode = rootNode->getNode("Materials");
	if(tempNode != NULL)
	{
		valueNode = tempNode->getNode("Sun Image");
		if(valueNode != NULL)
			sunImageName = valueNode->value->getString();
		else
		{
			throw Message::messageCode(MSG_ERROR, MSG_SKY, "No sun image found.");
		}
		valueNode = tempNode->getNode("Moon Image");
		if(valueNode != NULL)
			moonImageName = valueNode->value->getString();
		else
		{
			throw Message::messageCode(MSG_ERROR, MSG_SKY, "No moon image found.");
		}
		valueNode = tempNode->getNode("Sun Glow Map");
		if(valueNode != NULL)
			glowMapName = valueNode->value->getString();
		else
		{
			throw Message::messageCode(MSG_ERROR, MSG_SKY, "No sun glow map found.");
		}
		valueNode = tempNode->getNode("Sky Color Map");
		if(valueNode != NULL)
			colorMapName = valueNode->value->getString();
		else
		{
			throw Message::messageCode(MSG_ERROR, MSG_SKY, "No sky color map found.");
		}
	}
	else
	{
		throw Message::messageCode(MSG_ERROR, MSG_SKY, "No materials found.");
	}

	tempNode = rootNode->getNode("Shaders");
	if(tempNode != NULL)
	{
		valueNode = tempNode->getNode("Vertex");
		if(valueNode != NULL)
			vertexShaderFileName = valueNode->value->getString();
		else
		{
			throw Message::messageCode(MSG_ERROR, MSG_SKY, "No vertex shader found.");
		}
		valueNode = tempNode->getNode("Fragment");
		if(valueNode != NULL)
			fragmentShaderFileName = valueNode->value->getString();
		else
		{
			throw Message::messageCode(MSG_ERROR, MSG_SKY, "No fragment shader found.");
		}
	}
	else
	{
		throw Message::messageCode(MSG_ERROR, MSG_SKY, "No shaders found.");
	}

	tempNode = rootNode->getNode("Position");
	if(tempNode != NULL)
	{
		valueNode = tempNode->getNode("Skydome offset");
		if(valueNode != NULL)
			skydomeOffset = valueNode->value->getVec3f();
		else
		{
			Message::show(MSG_WARNING, MSG_SKY, "No skydome offset has been specified.");
		}
		valueNode = tempNode->getNode("Skyobjects offset");
		if(valueNode != NULL)
			skyObjectsOffset = valueNode->value->getVec3f();
		else
		{
			Message::show(MSG_WARNING, MSG_SKY, "No skyobjects offset has been specified.");
		}
	}
	else
	{
		Message::show(MSG_WARNING, MSG_SKY, "No skydome positions has been specified.");
	}
}