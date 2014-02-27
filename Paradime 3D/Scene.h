#ifndef SCENE_CLASS
#define SCENE_CLASS

#include <string>
#include <vector>

#include "Billboard.h"
#include "GameObjects.h"
#include "GeometryBuffer.h"
#include "Lighting.h"
#include "Loader_Config.h"
#include "Player.h"
#include "Skybox.h"
#include "Skydome.h"

class Scene
{
public:
	
	Player player;
	Lighting *lighting;

	ModelLoader::GenericModel		*testSphere;
	ShaderLoader::VertFragShader	*testShader;

	Scene();
	Scene(std::string sceneName_arg);
	~Scene();

	void loadScene(std::string sceneName_arg);
	void unloadScene();
	
	void renderSky();
	void renderObjects();
	void renderObjects(ShaderLoader::BaseShader *shader_arg);
	void update();

private:
	Sky			*sky;
	GameObjects *gameObjectPool;
	
	ConfigFile sceneFile;
	//std::string sceneName;

	void loadFromFile();
	void unloadGameObjects();

	void loadGameObjects(ConfigFile::Node *node_arg);
	void loadLighting(ConfigFile::Node *node_arg);
	void loadPlayer(ConfigFile::Node *node_arg);
	void loadSky(ConfigFile::Node *node_arg);
};

#endif