#ifndef CURRENT_UPDATE_STATE_NAMESPACE
#define CURRENT_UPDATE_STATE_NAMESPACE

#include "CommonClasses.h"
#include "ErrorHandler.h"
#include "Lighting.h"
#include "Player.h"
#include "Scene.h"

namespace Current
{
	extern Common::ObjectParameters *objectParameters;
	extern Common::Perspective *perspective;
	extern Common::InputState *inputState;
	extern Player *player;
	extern Scene *scene;
	extern double deltaTime;
	extern Lighting *lighting;
	extern Lighting::PointLight *pointLight;
	extern Lighting::SpotLight *spotLight;

	extern GLuint	positionMap,
					diffuseMap,
					normalMap,
					sunGlowMap,
					skyMap,
					dirShadowMap;

	void init();
}

#endif