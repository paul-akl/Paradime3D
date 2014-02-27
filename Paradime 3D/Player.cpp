
#include "CurrentUpdateState.h"
#include "ErrorHandler.h"
#include "OS_Layer.h"
#include "Player.h"

Player::Player()
{
	internalUpdate = &Player::updateFreeLook;

	currentCamera = new CameraEntity();

	upVector = Math3d::Vec3f(0, 1, 0);

	horizontalAngle = 0.0f;
	verticalAngle	= 0.0f;

	Message::show(MSG_INFO, MSG_PLAYER, "Created a new player at world origin");
}
Player::Player(Math3d::Vec3f position_arg)
{
	position = position_arg;
	targetVector = Math3d::Vec3f(1.0f, 1.0f, 1.0f);
	upVector = Math3d::Vec3f(0, 1, 0);

	//update = &updateFreeLook;

	currentCamera = new CameraEntity(position_arg);

	Message::show(MSG_INFO, MSG_PLAYER, "Created a new player at position: (" + Message::toString(position.x) + ", " + Message::toString(position.y) + ", " + Message::toString(position.z) + ").");
}
Player::Player(ModelLoader::Model *playerModel_arg)
{
	playerModel = playerModel_arg;
	//update = &updatePerson;
	upVector = Math3d::Vec3f(0, 1, 0);

	Message::show(MSG_INFO, MSG_PLAYER, "Created a new player, bound to model: " + playerModel->modelName);
}

Player::~Player()
{
	delete currentCamera;
}

void Player::update()
{
	(this->*internalUpdate)();
}

void Player::updateFreeLook()
{
	updateAngles();
		
	targetVector.target(verticalAngle, horizontalAngle);
	horizontalVector.horizontal(horizontalAngle);

	if(Current::inputState->forwardKey.status)
	{
		position += targetVector * Config::game::camera_freelook_speed * (float)Current::deltaTime;
	}
	if(Current::inputState->backwardKey.status)
	{
		position -= targetVector * Config::game::camera_freelook_speed * (float)Current::deltaTime;
	}
	if(Current::inputState->rightKey.status)
	{
		position += horizontalVector * Config::game::camera_freelook_speed * (float)Current::deltaTime;
	}
	if(Current::inputState->leftKey.status)
	{
		position -= horizontalVector * Config::game::camera_freelook_speed * (float)Current::deltaTime;
	}

	upVector = Math3d::cross(horizontalVector, targetVector);

	currentCamera->update(&position, &Math3d::Vec3f(position + targetVector), &upVector);
	currentCamera->updateViewMatrix();
}
void Player::updatePerson()
{

}
	
void Player::updateAngles()
{	
	if(Config::window::mouse_clipped)
	{
		horizontalAngle += (Config::keys::mouse_sensitivity * (Config::currentScreenWidth / 2 - Current::inputState->mousePosX));
		verticalAngle   += (Config::keys::mouse_sensitivity * (Config::currentScreenHeight / 2 - Current::inputState->mousePosY));

		OS::resetMousePosition();
	}
		

	//std::cout << "MOUSE X: " << (Config::currentScreenWidth / 2 - Current::inputState->mousePosX) << std::endl;
	//std::cout << "MOUSE Y: " << Config::keys::mouse_sensitivity << std::endl;

	//Current::inputState->mouseEvent = false;

	//int mouseX, mouseY;
			
	//SDL_GetMouseState(&mouseX, &mouseY);

	//horizontalAngle += (0.001f * (800 / 2.0f - Current::inputState->mousePosX));
	//verticalAngle   += (0.001f * (600 / 2.0f - Current::inputState->mousePosY));

	//std::cout << "MOUSE X: " << verticalAngle << std::endl;
	//std::cout << "MOUSE Y: " << horizontalAngle << std::endl;
}