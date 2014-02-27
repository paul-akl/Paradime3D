#include <fstream>
#include <iostream>

#include "Config.h"
#include "CurrentUpdateState.h"
#include "ErrorHandler.h"
#include "GameObjects.h"

#include "Skybox.h"

Skybox::Skybox(std::string fileName_arg) : Sky(fileName_arg)
{
	//fileName = fileName_arg;
	Message::show(MSG_INFO, MSG_SKY, "Handle has been created.");
}
Skybox::~Skybox()
{
	delete skyCubeMap;
	texturePool->clear();
	delete texturePool;
}

void Skybox::load()
{
	// Create and bind the Vertex Array Object
	glGenVertexArrays(1, &VAO);   
	glBindVertexArray(VAO);

	try
	{
		loadFromFile();
		skyShader = ShaderLoader::load(vertexShaderFileName, fragmentShaderFileName);
		skyModel = new ModelLoader::GenericModel(skyModelName);
		skyModelParam = new Common::ObjectParameters();
	}
	catch(Message::messageCode error)
	{
		error.display();
	}

	skyModelParam->scaleVec = Math3d::Vec3f(50, 50, 50);
	skyModelParam->rotationVec = Math3d::Vec3f(180, 0, 0);
	skyModelParam->modified = true;

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);
}
void Skybox::update()
{
	skyModelParam->modified = true;
	skyModelParam->positionVec = Current::player->currentCamera->position;
}
void Skybox::render()
{
	// Bind Vertex Array Object
	glBindVertexArray(VAO);

	Current::objectParameters = skyModelParam;
	skyCubeMap->bind();

	glDepthMask(false);
	glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);
	
	skyShader->updateFrame();

	skyModel->render(skyShader);

	glCullFace(Config::engine::face_culling_mode);
    glDepthFunc(Config::engine::depth_test_func);
	
	glDepthMask(true);

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);
}

void Skybox::loadFromFile()
{
	std::ifstream skyboxFile;
	std::string singleWord, nextWord;
	std::vector<std::string> parameterWords;
	bool lastPartFound = false;

	Common::Cubemap cubemap;
	cubemap.cubemapName = fileName;

	skyboxFile.open(fileName, std::ios::in);
	
	if(skyboxFile.fail())
	{
		throw Message::messageCode(MSG_ERROR, MSG_SKY, fileName + ": has failed to load.");
	}

	while(!skyboxFile.eof())
	{
		skyboxFile >> singleWord;

		lastPartFound = false;

		if(*singleWord.begin() == '"')
		{
			while(!lastPartFound)
			{
				skyboxFile >> nextWord;
				singleWord+= " " + nextWord;
					
				if(*singleWord.rbegin() == '"')
				{
					singleWord.erase(0, 1);
					singleWord.erase(singleWord.size() - 1, singleWord.size());
						
					lastPartFound = true;
				}
				else
					if(skyboxFile.eof())
						throw Message::messageCode(MSG_ERROR, MSG_OBJECT, fileName + ": Object file is missing \" (Quotation marks).");
			}
		}

		parameterWords.push_back(singleWord);
	}
		
	skyboxFile.close();

	for(unsigned int i=0; i < parameterWords.size(); i++)
	{
		if(parameterWords[i] == "posX")
		{
			cubemap.textureNames[0] = parameterWords[i + 1];
			i++;
			continue;
		}

		if(parameterWords[i] == "negX")
		{
			cubemap.textureNames[1] = parameterWords[i + 1];
			i++;
			continue;
		}

		if(parameterWords[i] == "posY")
		{
			cubemap.textureNames[2] = parameterWords[i + 1];
			i++;
			continue;
		}
			
		if(parameterWords[i] == "negY")
		{
			cubemap.textureNames[3] = parameterWords[i + 1];
			i++;
			continue;
		}
		
		if(parameterWords[i] == "posZ")
		{
			cubemap.textureNames[4] = parameterWords[i + 1];
			i++;
			continue;
		}
		
		if(parameterWords[i] == "negZ")
		{
			cubemap.textureNames[5] = parameterWords[i + 1];
			i++;
			continue;
		}

		if(parameterWords[i] == "vertex_shader")
		{
			vertexShaderFileName = parameterWords[i + 1];
			i++;
			continue;
		}

		if(parameterWords[i] == "fragment_shader")
		{
			fragmentShaderFileName = parameterWords[i + 1];
			i++;
			continue;
		}
		
		if(parameterWords[i] == "model")
		{
			skyModelName = parameterWords[i + 1];
			i++;
			continue;
		}
	}
	skyCubeMap = TextureLoader::load3D(&cubemap);
}