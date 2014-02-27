#include <fstream>
#include <iostream>
#include <sstream>

#include "Scene.h"

Scene::Scene()
{
	gameObjectPool = new GameObjects();
	lighting = new Lighting();

	try
	{
		gameObjectPool->init();
		lighting->init();
		testSphere = new ModelLoader::GenericModel("sphere.obj");
		testShader = ShaderLoader::load("geometryPass.pvert", "geometryPass.pfrag");
	}
	catch(Message::messageCode error_arg)
	{
		error_arg.display();
	}
}
Scene::Scene(std::string sceneName_arg)
{
	gameObjectPool = new GameObjects();
	lighting = new Lighting();
	loadScene(sceneName_arg);
}
Scene::~Scene()
{
	delete gameObjectPool;
	delete lighting;
	delete sky;
	delete testSphere;
	delete testShader;
}

void Scene::loadScene(std::string sceneName_arg)
{
	//unloadGameObjects();
	sceneFile.configFileName = sceneName_arg;
	loadFromFile();

	std::cout << "Number of point lights: " << lighting->pointLightPoolSize << std::endl;
}
void Scene::unloadScene()
{

}

void Scene::renderSky()
{
	sky->render();
}
void Scene::renderObjects()
{
	gameObjectPool->render();
}
void Scene::renderObjects(ShaderLoader::BaseShader *shader_arg)
{
	gameObjectPool->render(shader_arg);
}
void Scene::update()
{
	sky->update();

	gameObjectPool->update();
}

void Scene::loadFromFile()
{
	sceneFile.import();
	std::cout << sceneFile.getString() << std::endl;	// REMOVE
	
	ConfigFile::Node *rootNode = sceneFile.getRootNode();

	try
	{
		loadGameObjects(rootNode->getNode("game objects"));
		loadLighting(rootNode->getNode("lighting"));
		loadPlayer(rootNode->getNode("player"));
		loadSky(rootNode->getNode("sky"));
	}
	catch(Message::messageCode error)
	{
		error.display();
	}
}
void Scene::unloadGameObjects()
{
	//for(int i=0; i < modelPool.size(); i++)
		//ModelLoader::unload(modelPool[i]->modelName);

	//modelPool.clear();
}

void Scene::loadGameObjects(ConfigFile::Node* node_arg)
{
	if(node_arg != NULL)
	{
		ConfigFile::Node *tempNode;
		std::string tempName;
		Math3d::Vec3f tempPosition, tempRotation, tempScale;

		for(std::vector<ConfigFile::ArrayNode*>::size_type i=0; i < node_arg->arrayNodes.size(); i++)
		{
			tempNode = node_arg->arrayNodes[i]->getValueNode("name");
			if(tempNode != NULL)
				tempName = tempNode->value->getString();
			else
				tempName = "";

			tempNode = node_arg->arrayNodes[i]->getValueNode("position");
			if(tempNode != NULL)
				tempPosition = tempNode->value->getVec3f();

			tempNode = node_arg->arrayNodes[i]->getValueNode("rotation");
			if(tempNode != NULL)
				tempRotation = tempNode->value->getVec3f();

			tempNode = node_arg->arrayNodes[i]->getValueNode("scale");
			if(tempNode != NULL)
				tempScale = tempNode->value->getVec3f();

			gameObjectPool->load(tempName, tempPosition, tempRotation, tempScale);
		}
	}
}
void Scene::loadLighting(ConfigFile::Node* node_arg)
{
	if(node_arg != NULL)
	{
		ConfigFile::Node *tempNode, *tempLightNode;
		Math3d::Vec3f tempColor, tempPosition, tempDirection;
		float tempAmbientIntensity, tempDiffuseIntensity, tempConstant, tempLinear, tempQuadratic, tempCutoff;

		tempNode = node_arg->getNode("directional");
		if(tempNode != NULL)
		{
			tempLightNode = tempNode->getValueNode("color");
			if(tempLightNode != NULL)
				tempColor = tempLightNode->value->getVec3f();
			else
				tempColor = Math3d::Vec3f(0.0);

			tempLightNode = tempNode->getValueNode("direction");
			if(tempLightNode != NULL)
				tempDirection = tempLightNode->value->getVec3f();
			else
				tempDirection = Math3d::Vec3f(0.0);

			tempLightNode = tempNode->getValueNode("ambient intensity");
			if(tempLightNode != NULL)
				tempAmbientIntensity = tempLightNode->value->getFloat();
			else
				tempAmbientIntensity = 0.0f;

			tempLightNode = tempNode->getValueNode("diffuse intensity");
			if(tempLightNode != NULL)
				tempDiffuseIntensity = tempLightNode->value->getFloat();
			else
				tempDiffuseIntensity = 0.0f;

			lighting->addDirectionalLight(tempColor, tempAmbientIntensity, tempDiffuseIntensity, tempDirection);
		}
		else
		{
			lighting->addDirectionalLight(Math3d::Vec3f(0.0f), 0.0f, 0.0f, Math3d::Vec3f(0.0f));
		}
		
		tempNode = node_arg->getNode("point");
		for(std::vector<ConfigFile::Node*>::size_type i=0; i < tempNode->arrayNodes.size(); i++)
		{
			tempLightNode = tempNode->arrayNodes[i]->getValueNode("color");
			if(tempLightNode != NULL)
				tempColor = tempLightNode->value->getVec3f();
			else
				tempColor = Math3d::Vec3f(0.0);

			tempLightNode = tempNode->arrayNodes[i]->getValueNode("position");
			if(tempLightNode != NULL)
				tempPosition = tempLightNode->value->getVec3f();
			else
				tempPosition = Math3d::Vec3f(0.0);

			tempLightNode = tempNode->arrayNodes[i]->getValueNode("ambient intensity");
			if(tempLightNode != NULL)
				tempAmbientIntensity = tempLightNode->value->getFloat();
			else
				tempAmbientIntensity = 0.0f;

			tempLightNode = tempNode->arrayNodes[i]->getValueNode("diffuse intensity");
			if(tempLightNode != NULL)
				tempDiffuseIntensity = tempLightNode->value->getFloat();
			else
				tempDiffuseIntensity = 0.0f;

			tempLightNode = tempNode->arrayNodes[i]->getValueNode("constant attenuation");
			if(tempLightNode != NULL)
				tempConstant = tempLightNode->value->getFloat();
			else
				tempConstant = 0.0f;

			tempLightNode = tempNode->arrayNodes[i]->getValueNode("linear attenuation");
			if(tempLightNode != NULL)
				tempLinear = tempLightNode->value->getFloat();
			else
				tempLinear = 0.0f;

			tempLightNode = tempNode->arrayNodes[i]->getValueNode("quadratic attenuation");
			if(tempLightNode != NULL)
				tempQuadratic = tempLightNode->value->getFloat();
			else
				tempQuadratic = 0.0f;

			lighting->addPointLight(tempColor, tempAmbientIntensity, tempDiffuseIntensity, tempPosition, tempConstant, tempLinear, tempQuadratic);
		}

		tempNode = node_arg->getNode("spot");
		for(std::vector<ConfigFile::Node*>::size_type i=0; i < tempNode->arrayNodes.size(); i++)
		{
			tempLightNode = tempNode->arrayNodes[i]->getValueNode("color");
			if(tempLightNode != NULL)
				tempColor = tempLightNode->value->getVec3f();
			else
				tempColor = Math3d::Vec3f(0.0);

			tempLightNode = tempNode->arrayNodes[i]->getValueNode("position");
			if(tempLightNode != NULL)
				tempPosition = tempLightNode->value->getVec3f();
			else
				tempPosition = Math3d::Vec3f(0.0);

			tempLightNode = tempNode->arrayNodes[i]->getValueNode("direction");
			if(tempLightNode != NULL)
				tempDirection = tempLightNode->value->getVec3f();
			else
				tempDirection = Math3d::Vec3f(0.0);
			
			tempLightNode = tempNode->arrayNodes[i]->getValueNode("cutoff");
			if(tempLightNode != NULL)
				tempCutoff = tempLightNode->value->getFloat();
			else
				tempCutoff = 0.0f;

			tempLightNode = tempNode->arrayNodes[i]->getValueNode("ambient intensity");
			if(tempLightNode != NULL)
				tempAmbientIntensity = tempLightNode->value->getFloat();
			else
				tempAmbientIntensity = 0.0f;

			tempLightNode = tempNode->arrayNodes[i]->getValueNode("diffuse intensity");
			if(tempLightNode != NULL)
				tempDiffuseIntensity = tempLightNode->value->getFloat();
			else
				tempDiffuseIntensity = 0.0f;

			tempLightNode = tempNode->arrayNodes[i]->getValueNode("constant attenuation");
			if(tempLightNode != NULL)
				tempConstant = tempLightNode->value->getFloat();
			else
				tempConstant = 0.0f;

			tempLightNode = tempNode->arrayNodes[i]->getValueNode("linear attenuation");
			if(tempLightNode != NULL)
				tempLinear = tempLightNode->value->getFloat();
			else
				tempLinear = 0.0f;

			tempLightNode = tempNode->arrayNodes[i]->getValueNode("quadratic attenuation");
			if(tempLightNode != NULL)
				tempQuadratic = tempLightNode->value->getFloat();
			else
				tempQuadratic = 0.0f;

			lighting->addSpotLight(tempColor, tempAmbientIntensity, tempDiffuseIntensity, tempPosition, tempConstant, tempLinear, tempQuadratic, tempDirection, tempCutoff);
		}
	}
}
void Scene::loadPlayer(ConfigFile::Node *node_arg)
{
	if(node_arg != NULL)
	{
		ConfigFile::Node *tempNode;
		Math3d::Vec3f tempPosition;

		tempNode = node_arg->getValueNode("position");
		tempPosition = tempNode->value->getVec3f();

		player.position = tempPosition;
	}
	else
	{
		player.position = Math3d::Vec3f(0.0f);
	}
}
void Scene::loadSky(ConfigFile::Node *node_arg)
{
	if(node_arg != NULL)
	{
		ConfigFile::Node *tempNode, *nameNode = node_arg->getValueNode("name");
		
		tempNode = node_arg->getValueNode("type");
		if(tempNode->value->getString() == "skydome")
		{
			sky = new Skydome(nameNode->value->getString());
			sky->load();
		}
		else
		{
			if(tempNode->value->getString() == "skybox")
			{

			}
			else
			{
				throw Message::messageCode(MSG_ERROR, MSG_SCENE, "No sky found.");
			}
		}
	}
	else
	{
		throw Message::messageCode(MSG_ERROR, MSG_SCENE, "No sky found.");
	}
}