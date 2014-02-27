#include <sstream>

#include "Clock.h"
#include "Config.h"
#include "ErrorHandler.h"
#include "GameObjects.h"
#include "Game.h"
#include "Loader_Texture.h"
#include "Loader_Shader.h"
#include "OS_Layer.h"

//#include "state_Intro.h"
//#include "state_MainMenu.h"
#include "state_Play.h"

// Static data member of class
int Game::instances = 0;

Game::Game(void) // Constructor method
{
	Message::init();

	// We should only have ONE instance of the game class
	instances++;
	if (instances > 1)
		Message::show(MSG_ERROR, MSG_ENGINE, "Attempt to create multiple game instances.");
	
	Clock::init();
	Config::init();
	OS::setupRC();
	TextureLoader::init();
}
Game::~Game() // Perform any required clean-up here
{
	//TTF_CloseFont(textFont);
    //SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::init(void) // Initialize OpenGL values and game related values and variables
{	
	glewExperimental = GL_TRUE;
	GLuint glewError = glewInit();

	if(glewError != GLEW_OK)
	{
		std::stringstream stringstreamGlewError;
		stringstreamGlewError << glewGetErrorString(glewError);
		Message::show(MSG_ERROR, MSG_ENGINE, "GLEW Initialization failed: " + stringstreamGlewError.str());
	}

	glClearColor(0.0, 0.0, 0.0, 0.0); // set background colour to black for light blending
	//glClearColor(0.2, 0.2, 0.2, 0.0);

	ModelLoader::init();
	ShaderLoader::init();
	TextureLoader::init();
	
	playState = new StatePlay();
	playState->init(*this);
	//pause(1);

	//mainMenuState = new StateMainMenu();
	//mainMenuState->init(*this);
	//pause(1);

	//introState = new StateIntro();
	//introState->init(*this);
	//pause(1);

	currentState = playState;

	Current::init();
	std::srand((unsigned int)std::time(NULL));
}
void Game::globalUpdate()
{

}
void Game::run(void) // This function contains the main game loop
{
	Message::show(MSG_INFO, MSG_ENGINE, "About to enter main loop.");

	while (Config::running)	// the event loop
	{
		OS::handleEvents();

		Clock::Update();

		currentState->update(*this);
		currentState->draw();
	}
}

void Game::setState(GameState * newState)
{
	currentState = newState;
}
GameState * Game::getState(void)
{
	return currentState;
}
GameState * Game::getPlayState(void)
{
	return playState;
}
/*
GameState * Game::getMainMenuState(void)
{
	return mainMenuState;
}

GameState * Game::getIntroState(void)
{
	return introState;
}
*/