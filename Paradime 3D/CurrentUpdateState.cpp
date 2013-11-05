
#include "CurrentUpdateState.h"

namespace Current
{
	Common::ObjectParameters *objectParameters = NULL;
	Common::Perspective *perspective = NULL;
	Common::InputState *inputState = NULL;
	Player *player = NULL;
	Scene *scene = NULL;
	Lighting *lighting;
	Lighting::PointLight *pointLight = NULL;
	Lighting::SpotLight *spotLight = NULL;
	double deltaTime = 0;
	
	GLuint	positionMap = 0,
			diffuseMap = 0,
			normalMap = 0;


	void init()
	{

	}
}