#include <iostream>
//#include <crtdbg.h>	// Checking for memory leaks

#include "Game.h"

#if _DEBUG
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif

int main(int argc, char *argv[])
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	// Checking for memory leaks

	Game *newGame = new Game();

	newGame->init(); // Initialize the OpenGL and game variables
	newGame->run();

	delete newGame;
    return 0;
}