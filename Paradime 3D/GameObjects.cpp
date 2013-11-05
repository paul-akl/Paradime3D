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
	for(int i=0; i < objectPool.size(); i++)
		objectPool[i]->render();
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
void GameObjects::Object::update()
{

}
bool GameObjects::Object::compareFileName(std::string fileName_arg)
{
	return (objectFileName == fileName_arg);
}
void GameObjects::Object::loadFromFile(std::string fileName_arg)
{
	std::ifstream configFile;
	std::string singleWord, nextWord;
	std::vector<std::string> parameterWords;
	bool lastPartFound = false;

	configFile.open(fileName_arg, std::ios::in);
	
	if(configFile.fail())
	{
		throw Message::messageCode(MSG_FATAL_ERROR, MSG_OBJECT, fileName_arg + ": Has failed to load.");
	}
	
	while(!configFile.eof())
	{
		configFile >> singleWord;

		lastPartFound = false;

		if(*singleWord.begin() == '"')
		{
			while(!lastPartFound)
			{
				configFile >> nextWord;
				singleWord+= " " + nextWord;
					
				if(*singleWord.rbegin() == '"')
				{
					singleWord.erase(0, 1);
					singleWord.erase(singleWord.size() - 1, singleWord.size());
						
					lastPartFound = true;
				}
				else
					if(configFile.eof())
						throw Message::messageCode(MSG_ERROR, MSG_OBJECT, fileName_arg + ": Object file is missing \" (Quotation marks).");
			}
		}

		parameterWords.push_back(singleWord);
	}
		
	configFile.close();

	for(unsigned int i=0; i < parameterWords.size(); i++)
	{
		if(parameterWords[i] == "material")
		{
			objectLoaderParam.materialsFromFile.push_back(new ObjectLoaderParameters::MaterialNameAndIndex(parameterWords[i+2], std::atoi(parameterWords[i+1].c_str())));
			i += 2;
			continue;
		}

		if(parameterWords[i] == "model")
		{
			objectLoaderParam.modelName = parameterWords[i+1];
			i++;
			continue;
		}

		if(parameterWords[i] == "vertex_shader")
		{
			objectLoaderParam.vertexShaderName = parameterWords[i+1];
			i++;
			continue;
		}
			
		if(parameterWords[i] == "fragment_shader")
		{
			objectLoaderParam.fragmentShaderName = parameterWords[i+1];
			i++;
			continue;
		}
			
		if(parameterWords[i] == "rotation")
		{
			objectParameters->rotationVec = Math3d::Vec3f(std::atof(parameterWords[i+1].c_str()), std::atof(parameterWords[i+2].c_str()), std::atof(parameterWords[i+3].c_str()));
			i += 3;
			continue;
		}

		if(parameterWords[i] == "scale")
		{
			objectParameters->scaleVec = Math3d::Vec3f(std::atof(parameterWords[i+1].c_str()), std::atof(parameterWords[i+2].c_str()), std::atof(parameterWords[i+3].c_str()));
			i += 3;
			continue;
		}
	}
}