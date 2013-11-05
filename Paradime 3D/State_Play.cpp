#include <functional>
#include <iostream>
#include <sstream>
#include <string>

// While a declaration of class Game has been provided in
// gamestate.h, this was only a forward declaration
// because we need to work with the implementation now
// we need to include game.h here

#include "Clock.h"
#include "Config.h"
#include "ErrorHandler.h"
#include "Game.h"
#include "GameObjects.h"
#include "state_Play.h"


#include "Math.h"
#include <iostream>

StatePlay::StatePlay(void)
{

}


StatePlay::~StatePlay(void)
{

}

void displayFPS()
{
	OS::setWindowTitle(Config::window::name + " | " + Clock::GetFPSstring() + " | " + Clock::GetMSstring());
}

void StatePlay::init(Game &context)
{
	Message::show(MSG_INFO, MSG_ENGINE, "Initializing play state.");

	inputs.init();

	Current::inputState = &inputs;
	Current::perspective = &perspective;
	Current::scene = new Scene();
	Current::scene->loadScene(Config::path::maps_path + "test.pmap");
	Current::player = &Current::scene->player;

	renderer.init();

	Clock::CreateTimer(&displayFPS, 0, true, 200);
}

void StatePlay::update(Game &context)
{
	Current::player->update();

	Current::scene->update();
}

void StatePlay::draw()
{	
	// Update all the uniforms required for this frame
	ShaderLoader::updateFrame();
	
	//Current::scene->render(); // Render the current map/scene (includes models, skybox, etc.)
	renderer.renderScene();

	OS::swapBuffers(); // Swap the OpenGL buffers
}

void StatePlay::handleSDLEvent(SDL_Event const &sdlEvent, Game &context)
{

}
