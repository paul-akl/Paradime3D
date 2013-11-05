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
	//delete gameObjectPool;
	//delete lighting;
}

void Scene::loadScene(std::string sceneName_arg)
{
	//unloadGameObjects();
	sceneName = sceneName_arg;
	loadFromFile();
}
void Scene::unloadScene()
{

}

void Scene::renderSkybox()
{
	skybox->render();
}
void Scene::renderObjects()
{
	gameObjectPool->render();
}
void Scene::update()
{
	skybox->update();

	gameObjectPool->update();
}

void Scene::loadFromFile()
{
	std::ifstream sceneFile;
	std::string singleWord, nextWord;
	std::vector<std::string> parameterWords;
	bool lastPartFound = false;

	sceneFile.open(sceneName, std::ios::in);

	if(sceneFile.fail())
	{
		throw Message::messageCode(MSG_FATAL_ERROR, MSG_OBJECT, sceneName + ": has failed to load.");
	}

	while(!sceneFile.eof())
	{
		sceneFile >> singleWord;

		lastPartFound = false;

		if(*singleWord.begin() == '"')
		{
			while(!lastPartFound)
			{
				sceneFile >> nextWord;
				singleWord+= " " + nextWord;
					
				if(*singleWord.rbegin() == '"')
				{
					singleWord.erase(0, 1);
					singleWord.erase(singleWord.size() - 1, singleWord.size());
						
					lastPartFound = true;
				}
				else
					if(sceneFile.eof())
						throw Message::messageCode(MSG_ERROR, MSG_OBJECT, sceneName + ": Scene file is missing \" (Quotation marks).");
			}
		}

		parameterWords.push_back(singleWord);
	}
		
	sceneFile.close();

	std::string dirLightVertexShader		= "", 
				dirLightFragmentShader		= "",
				pointLightVertexShader		= "",
				pointLightFragmentShader	= "",
				spotLightVertexShader		= "",
				spotLightFragmentShader		= "",
				stencilPassVertexShader		= "",
				stencilPassFragmentShader	= "";

	for(unsigned int i=0; i < parameterWords.size(); i++)
	{
		if(parameterWords[i] == "game_object" || parameterWords[i] == "game_obj")
		{			
			gameObjectPool->load(parameterWords[i+1],	Math3d::Vec3f(std::atof(parameterWords[i+2].c_str()), std::atof(parameterWords[i+3].c_str()), std::atof(parameterWords[i+4].c_str())),
														Math3d::Vec3f(std::atof(parameterWords[i+5].c_str()), std::atof(parameterWords[i+6].c_str()), std::atof(parameterWords[i+7].c_str())),
														Math3d::Vec3f(std::atof(parameterWords[i+8].c_str()), std::atof(parameterWords[i+9].c_str()), std::atof(parameterWords[i+10].c_str())));

			i += 10;
			continue;
		}

		if(parameterWords[i] == "skybox" || parameterWords[i] == "skysphere")
		{
			skybox = new Skybox(parameterWords[i+1]);
			skybox->load();
			i++;
			continue;
		}

		if(parameterWords[i] == "light_directional")
		{
			lighting->addDirectionalLight(	Math3d::Vec3f(std::atof(parameterWords[i+1].c_str()), std::atof(parameterWords[i+2].c_str()), std::atof(parameterWords[i+3].c_str())),
											std::atof(parameterWords[i+4].c_str()), std::atof(parameterWords[i+5].c_str()),
											Math3d::Vec3f(std::atof(parameterWords[i+6].c_str()), std::atof(parameterWords[i+7].c_str()), std::atof(parameterWords[i+8].c_str())));
			i += 8;
			
			continue;
		}
			
		if(parameterWords[i] == "light_point")
		{
			lighting->addPointLight(	Math3d::Vec3f(std::atof(parameterWords[i+1].c_str()), std::atof(parameterWords[i+2].c_str()), std::atof(parameterWords[i+3].c_str())),
										std::atof(parameterWords[i+4].c_str()), std::atof(parameterWords[i+5].c_str()),
										Math3d::Vec3f(std::atof(parameterWords[i+6].c_str()), std::atof(parameterWords[i+7].c_str()), std::atof(parameterWords[i+8].c_str())),
										std::atof(parameterWords[i+9].c_str()), std::atof(parameterWords[i+10].c_str()), std::atof(parameterWords[i+11].c_str())	);
			i += 11;
			continue;
		}
			
		if(parameterWords[i] == "light_spot")
		{
			//objectParameters->rotationVec = Math3d::Vec3f(std::atof(parameterWords[i+1].c_str()), std::atof(parameterWords[i+2].c_str()), std::atof(parameterWords[i+3].c_str()));
			i += 3;
			continue;
		}

		if(parameterWords[i] == "light_directional_shader")
		{
			dirLightVertexShader	= parameterWords[i+1];
			dirLightFragmentShader	= parameterWords[i+2];
			i += 2;
			continue;
		}

		if(parameterWords[i] == "light_point_shader")
		{
			pointLightVertexShader	 = parameterWords[i+1];
			pointLightFragmentShader = parameterWords[i+2];
			i += 2;
			continue;
		}

		if(parameterWords[i] == "light_spot_shader")
		{
			spotLightVertexShader	= parameterWords[i+1];
			spotLightFragmentShader	= parameterWords[i+2];
			i += 2;
			continue;
		}

		if(parameterWords[i] == "light_stencilPass_shader")
		{
			stencilPassVertexShader	= parameterWords[i+1];
			stencilPassFragmentShader	= parameterWords[i+2];
			i += 2;
			continue;
		}

		if(parameterWords[i] == "player")
		{
			player.position = Math3d::Vec3f(std::atof(parameterWords[i+1].c_str()), std::atof(parameterWords[i+2].c_str()), std::atof(parameterWords[i+3].c_str()));
			i += 3;
			continue;
		}
	}

	/*// Check if there aren't any missing shader names in the scene file
	// If not, initialize lighting shaders
	if(	dirLightVertexShader	== "" || dirLightFragmentShader		== "" || 
		pointLightVertexShader	== "" || pointLightFragmentShader	== "" || 
		spotLightVertexShader	== "" || spotLightFragmentShader	== ""	 )
		throw Message::messageCode(MSG_FATAL_ERROR, MSG_SCENE, "Missing lighting shaders in \"" + sceneName + "\".");
	//else
		//lighting->initShaders(dirLightVertexShader, dirLightFragmentShader, pointLightVertexShader, pointLightFragmentShader, spotLightVertexShader, spotLightFragmentShader);
	
	// Check if there aren't any missing shader names in the scene file
	// If not, initialize gbuffer stencil pass shaders
	if(stencilPassVertexShader == "" || stencilPassFragmentShader == "")
		throw Message::messageCode(MSG_FATAL_ERROR, MSG_SCENE, "Missing stencil pass shaders in \"" + sceneName + "\".");
	//else
		//gBuffer->initStencilShaders(stencilPassVertexShader, stencilPassFragmentShader);*/
}
void Scene::unloadGameObjects()
{
	//for(int i=0; i < modelPool.size(); i++)
		//ModelLoader::unload(modelPool[i]->modelName);

	//modelPool.clear();
}