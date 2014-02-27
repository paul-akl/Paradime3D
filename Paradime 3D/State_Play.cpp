

#include "Clock.h"
#include "Config.h"
#include "ErrorHandler.h"
#include "Game.h"
#include "GameObjects.h"
#include "Math.h"
#include "state_Play.h"
#include "ShadowMapDefRenderer.h"



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
	Current::scene->loadScene(Config::path::maps_path + "JSON.pmap");
	Current::player = &Current::scene->player;

	//renderer = new ShadowMapDefRenderer();
	renderer = new DeferredRenderer();
	renderer->init();

	Clock::CreateTimer(&displayFPS, 0, true, 200);
}

void StatePlay::update(Game &context)
{
	renderer->update();

	Current::player->update();
	
	//	Math3d::Vec3f pos = Math3d::Vec3f(-50.0, -50.0, -50.0);
		
	//	pos.rotate(Current::scene->lighting->directionalLight->direction.z, Math3d::Vec3f(0.0, 0.0, 1.0));
	//	pos.rotate(Current::scene->lighting->directionalLight->direction.x, Math3d::Vec3f(1.0, 0.0, 0.0));
	//	pos.rotate(Current::scene->lighting->directionalLight->direction.y, Math3d::Vec3f(0.0, 1.0, 0.0));

	//Current::player->currentCamera->viewMatrix.initCamera(pos + Current::player->position, Current::player->position, Math3d::Vec3f(0.0f, 1.0f, 0.0f));
	Current::scene->update();
}

void StatePlay::draw()
{	
	// Update all the uniforms required for this frame
	ShaderLoader::updateFrame();
	
	//Current::scene->render(); // Render the current map/scene (includes models, skybox, etc.)
	renderer->renderScene();

	OS::swapBuffers(); // Swap the OpenGL buffers
}

void StatePlay::handleSDLEvent(SDL_Event const &sdlEvent, Game &context)
{

}
