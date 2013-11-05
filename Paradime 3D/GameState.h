#ifndef GAME_STATE
#define GAME_STATE

#include <SDL.h>

// GameState needs to refer to the Game object which needs to
// refer to GameState objects, which creates a circular dependency
// and causes problems with #includes 
// solution is to declare (but not define) the Game type here.
// so instead of 
// #include "Game.h"
// we have a 'forward declaration' of the class:
class Game;
// Game.h will still need to be included in the state implementation files

// Abstract game state class
// different game states will inherit from this
class GameState {
public:
	virtual ~GameState() { return; } // need a virtual destructor
	virtual void draw() = 0;
	//virtual void init(Game * context) = 0;
	virtual void init(Game &context) = 0;
	// Not using update function yet
	virtual void update(Game &context) = 0;
	virtual void handleSDLEvent(SDL_Event const &sdlEvent, Game &context) = 0;
};

#endif