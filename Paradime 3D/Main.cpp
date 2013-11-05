// iostream for cin and cout
#include <iostream>
#include "Game.h"

#if _DEBUG
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif

// Program entry point
// Almost all functionality has been moved into the Game class
int main(int argc, char *argv[])
{
	Game *newGame = new Game();

	newGame->init(); // initialize the OpenGL and game variables
	newGame->run();

	delete newGame;
    return 0;
}