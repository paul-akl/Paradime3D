#ifndef SCENE_CLASS
#define SCENE_CLASS

#include <string>
#include <vector>

#include "GameObjects.h"
#include "GeometryBuffer.h"
#include "Lighting.h"
#include "Player.h"
#include "Skybox.h"

class Scene
{
public:
	
	Player player;
	Lighting *lighting;

	ModelLoader::GenericModel *testSphere;
	ShaderLoader::Shader	*testShader;

	Scene();
	Scene(std::string sceneName_arg);
	~Scene();

	void loadScene(std::string sceneName_arg);
	void unloadScene();
	
	void renderSkybox();
	void renderObjects();
	void update();

private:
	
	Skybox *skybox;
	GameObjects *gameObjectPool;

	std::string sceneName;

	void loadFromFile();
	void unloadGameObjects();
};

#endif