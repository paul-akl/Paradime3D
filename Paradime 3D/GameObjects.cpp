#include <fstream>
#include <iostream>
#include <sstream>

#include "CurrentUpdateState.h"
#include "GameObjects.h"

GameObjects::GameObjects()
{
	//init();
}
GameObjects::~GameObjects()
{
	objectPool.clear();
}

void GameObjects::init()
{

}
void GameObjects::uninit()
{
	//delete player;
}

void GameObjects::load(std::string fileName_arg, Math3d::Vec3f position_arg, Math3d::Vec3f rotation_arg, Math3d::Vec3f scale_arg)
{
	// No dublicate objects can exist, since they would share every parameter (including position), and would be indistinguishable
	// New instance in the object pool is required for every new object
	objectPool.push_back(new Object(fileName_arg));
	objectPoolSize = objectPool.size();

	objectPool[objectPoolSize - 1]->objectParameters->positionVec	+= position_arg;
	objectPool[objectPoolSize - 1]->objectParameters->rotationVec	+= rotation_arg;
	objectPool[objectPoolSize - 1]->objectParameters->scaleVec		*= scale_arg;
}
void GameObjects::unload(std::string fileName_arg)
{
	// Match the name of the object to remove
	for(int i=0; i < objectPoolSize; i++)
	{
		if(objectPool[i]->compareFileName(fileName_arg))
		{
			delete objectPool[i];
			objectPool.erase(objectPool.begin() + i);
		}
	}
	objectPoolSize = objectPool.size();
}

void GameObjects::setPlayer(Math3d::Vec3f *position_arg)
{

}
void GameObjects::setPlayer(Math3d::Vec3f *position_arg, Math3d::Vec3f *target_arg)
{

}

void GameObjects::render()
{
	// Update all the uniforms required for this frame
	//ShaderLoader::updateFrame();

	// Render each object in the object pool
	for(std::vector<Object*>::size_type i=0; i < objectPool.size(); i++)
		objectPool[i]->render();
}
void GameObjects::render(ShaderLoader::BaseShader *shader_arg)
{
	// Render each object in the object pool with a specified shader
	for(std::vector<Object*>::size_type i=0; i < objectPool.size(); i++)
		objectPool[i]->render(shader_arg);
}
void GameObjects::update()
{
	// Update each object in the object pool
	for(int i=0; i < objectPoolSize; i++)
		objectPool[i]->update();
}

GameObjects::Object::ObjectLoaderParameters::ObjectLoaderParameters()
{

}
GameObjects::Object::ObjectLoaderParameters::~ObjectLoaderParameters()
{

}

GameObjects::Object::Object(std::string fileName_arg)
{
	Message::show(MSG_INFO, MSG_OBJECT, fileName_arg + ": game object handle has been created.");

	objectFileName = fileName_arg;
	objectParameters = new Common::ObjectParameters();

	objectParameters->modified = true;

	try
	{
		loadFromFile(Config::path::objects_path + objectFileName);
		model = ModelLoader::load(objectLoaderParam.modelName);
		shader = ShaderLoader::load(objectLoaderParam.vertexShaderName, objectLoaderParam.fragmentShaderName);
	}
	catch(Message::messageCode error)
	{
		error.display();
	}
		
	texturePool.resize(model->materialPool->numMaterials);

	for(unsigned int i=0; i < model->materialPool->numMaterials; i++)
	{
		bool materialOverridden = false;
		try
		{
			//texturePool[i] = TextureLoader::load2D(objectLoaderParameters->getMaterial(i), objectLoaderParameters->getSpecularPower(i), objectLoaderParameters->getSpecularIntensity(i));
			for(unsigned int j=0; j < objectLoaderParam.materialsFromFile.size(); j++)
			{
				if(objectLoaderParam.materialsFromFile[j]->index == i)
				{
					texturePool[i] = TextureLoader::load2D(objectLoaderParam.materialsFromFile[j]->materialName);
					materialOverridden = true;
				}
			}
			if(!materialOverridden)
				if(i == 0)
					texturePool[i] = TextureLoader::load2Ddefault();
				else
					texturePool[i] = TextureLoader::load2D(model->materialPool->diffuseMaterials[i]);
				//texturePool[i] = TextureLoader::load2D(model->materialPool->getDiffuseMaterialName(i), Config::texture::default_specular_power, Config::texture::default_specular_intensity);
		}
		catch(Message::messageCode error)
		{
			error.display();
		}
	}

	// Make sure the VAO is not changed from the outside
	//glBindVertexArray(0);
		
	Message::show(MSG_INFO, MSG_OBJECT, fileName_arg + ": game object has been loaded sucessfully.");
}
GameObjects::Object::~Object()
{
	delete model;
	delete shader;
	delete objectParameters;
	for(std::vector<TextureLoader::Texture2D*>::size_type i=0; i < texturePool.size(); i++)
		TextureLoader::unload2D(texturePool[i]->fileName);

	Message::show(MSG_INFO, MSG_OBJECT, objectFileName + " has been removed from memory.");
}

void GameObjects::Object::render()
{		
	// Set the current object parameters required for updating uniforms
	//objectParameters->modified = true;
	Current::objectParameters = objectParameters;

	// Render the model
	model->render(&texturePool, shader);
}
void GameObjects::Object::render(ShaderLoader::BaseShader *shader_arg)
{
	// Set the current object parameters required for updating uniforms
	Current::objectParameters = objectParameters;

	// Render the model
	model->render(&texturePool, shader_arg);
}
void GameObjects::Object::update()
{

}
bool GameObjects::Object::compareFileName(std::string fileName_arg)
{
	return (objectFileName == fileName_arg);
}
void GameObjects::Object::loadFromFile(std::string fileName_arg)
{
	gameObjectFile.import(fileName_arg);

	ConfigFile::Node *tempNode, *valueNode,
					 *rootNode = gameObjectFile.getRootNode();

	tempNode = rootNode->getNode("Model");
	if(tempNode != NULL)
	{
		valueNode = tempNode->getNode("File name");
		if(valueNode != NULL)
			objectLoaderParam.modelName = valueNode->value->getString();
		else
		{
			// NO MODEL NAME ERROR
		}

		valueNode = tempNode->getNode("Rotation");
		if(valueNode != NULL)
			objectParameters->rotationVec = valueNode->value->getVec3f();

		valueNode = tempNode->getNode("Scale");
		if(valueNode != NULL)
			objectParameters->scaleVec = valueNode->value->getVec3f();
	}
	else
	{
		// NO MODEL ERROR
	}

	tempNode = rootNode->getNode("Shaders");
	if(tempNode != NULL)
	{
		valueNode = tempNode->getNode("Vertex");
		if(valueNode != NULL)
			objectLoaderParam.vertexShaderName = valueNode->value->getString();
		else
		{
			// NO VERTEX SHADER NAME ERROR
		}

		valueNode = tempNode->getNode("Fragment");
		if(valueNode != NULL)
			objectLoaderParam.fragmentShaderName = valueNode->value->getString();
		else
		{
			// NO FRAGMENT SHADER NAME ERROR
		}
	}
	else
	{
		// NO SHADER ERROR
	}

	tempNode = rootNode->getNode("Materials");
	if(tempNode != NULL)
	{
		int indexTemp = 0;

		tempNode = tempNode->getNode("Diffuse");
		if(tempNode != NULL)
		{
			for(std::vector<ConfigFile::ArrayNode*>::size_type i=0; i < tempNode->arrayNodes.size(); i++)
			{
				valueNode = tempNode->arrayNodes[i]->getNode("Index");
				if(valueNode != NULL)
					indexTemp = valueNode->value->getInt();
				else
				{
					// NO INDEX ERROR
				}

				valueNode = tempNode->arrayNodes[i]->getNode("Name");
				if(valueNode != NULL)
					objectLoaderParam.materialsFromFile.push_back(new ObjectLoaderParameters::MaterialNameAndIndex(valueNode->value->getString(), indexTemp));
				else
				{
					// NO MATERIAL NAME ERROR
				}
			}
		}
	
		tempNode = (rootNode->getNode("Materials"))->getNode("Normal");
		if(tempNode != NULL)
		{
			for(std::vector<ConfigFile::ArrayNode*>::size_type i=0; i < tempNode->arrayNodes.size(); i++)
			{
				valueNode = tempNode->arrayNodes[i]->getNode("Index");
				if(valueNode != NULL)
					indexTemp = valueNode->value->getInt();
				else
				{
					// NO INDEX ERROR
				}

				valueNode = tempNode->arrayNodes[i]->getNode("Name");
				if(valueNode != NULL)
				{
					// TODO: Add Normal Map
				}
				else
				{
					// NO MATERIAL NAME ERROR
				}
			}
		}
	}
}