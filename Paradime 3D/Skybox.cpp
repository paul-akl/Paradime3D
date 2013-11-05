#include <fstream>
#include <iostream>

#include "Config.h"
#include "CurrentUpdateState.h"
#include "ErrorHandler.h"
#include "GameObjects.h"

#include "Skybox.h"

Skybox::Skybox(std::string fileName_arg)
{
	fileName = fileName_arg;
	Message::show(MSG_INFO, MSG_SKYBOX, "Handle has been created.");
}
Skybox::~Skybox()
{

}

void Skybox::load()
{
	// Create and bind the Vertex Array Object
	glGenVertexArrays(1, &VAO);   
	glBindVertexArray(VAO);

	try
	{
		loadFromFile();
		shader = ShaderLoader::load(vertexShaderFileName, fragmentShaderFileName);
		sphere = new ModelLoader::GenericModel(modelName);
		objectParameters = new Common::ObjectParameters();
	}
	catch(Message::messageCode error)
	{
		error.display();
	}

	objectParameters->scaleVec = Math3d::Vec3f(50, 50, 50);
	objectParameters->rotationVec = Math3d::Vec3f(180, 0, 0);
	objectParameters->modified = true;

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);
}
void Skybox::update()
{
	objectParameters->modified = true;
	objectParameters->positionVec = Current::player->currentCamera->position;
}
void Skybox::render()
{
	// Bind Vertex Array Object
	glBindVertexArray(VAO);

	Current::objectParameters = objectParameters;
	texture3D->bind();

	glDepthMask(false);
	glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);
	
	shader->updateFrame();

	sphere->render(shader);

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
		throw Message::messageCode(MSG_ERROR, MSG_SKYBOX, fileName + ": has failed to load.");
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
			modelName = parameterWords[i + 1];
			i++;
			continue;
		}
	}
	texture3D = TextureLoader::load3D(&cubemap);
}