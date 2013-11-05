#ifndef GAME_MAIN
#define GAME_MAIN

#include <iostream>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

#include "GameState.h"
#include "label.h"
#include "OS_Layer.h"

#include "Clock.h"
#include "Timer.h"

class Game
{
public:

	Game(void);
	~Game();

	void init(void); // init and run are the only functions that need called from main
	void run(void);
	void globalUpdate(void);

	void setState(GameState * newState);
	GameState *getState(void);
	GameState *getPlayState(void);
	//GameState *getIntroState(void);
	//GameState *getMainMenuState(void);

private:

	GameState * playState;
	//GameState * introState;
	GameState * mainMenuState;
	GameState * currentState;

	SDL_Window *window;
	SDL_GLContext context;
	Common::InputState inputs;

	static int instances;

	TTF_Font * textFont;	// SDL type for True-Type font rendering
	
	std::string windowTitle;
	double deltaTime;
};

#endif