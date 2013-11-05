#ifndef PLAYER_CLASS
#define PLAYER_CLASS

#include <vector>

#include "Camera.h"
#include "Loader_Model.h"
#include "Math.h"

class Player
{

public:
	
	Player();
	Player(Math3d::Vec3f position_arg);
	Player(ModelLoader::Model *playerModel_arg);
	~Player();

	void update();

	void (Player::*internalUpdate)();
	 
	Math3d::Vec3f	position,
					targetVector,
					horizontalVector,
					upVector;
		
	float			horizontalAngle,
					verticalAngle;
		
	CameraEntity *currentCamera;
	ModelLoader::Model *playerModel;

private:

	void updateFreeLook();
	void updatePerson();
	//void updateCar();

	void updateAngles();

};

#endif