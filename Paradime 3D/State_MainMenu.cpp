#include "state_MainMenu.h"
#include <iostream>

// While a declaration of class Game has been provided in
// gamestate.h, this was only a forward declaration
// because we need to work with the implementation now
// we need to include game.h here
#include "Game.h"


StateMainMenu::StateMainMenu(void)
{
}

StateMainMenu::~StateMainMenu(void)
{
}

void StateMainMenu::init(Game &context)
{
	std::cout << "Initializing main menu state." << std::endl;
}

void StateMainMenu::update(float deltaTime, unsigned int currentMS, Game &context)
{

}

void StateMainMenu::draw(SDL_Window * window)
{

}

void StateMainMenu::handleSDLEvent(SDL_Event const &sdlEvent, Game &context)
{

}


