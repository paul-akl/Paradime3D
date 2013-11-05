#ifndef STATE_MAIN_MENU
#define STATE_MAIN_MENU

#include "GameState.h"
#include "Label.h"

class StateMainMenu : public GameState
{
public:
	StateMainMenu(void);
	~StateMainMenu(void);
	void init(Game &context);
	void draw(SDL_Window * window);
	void update(float deltaTime, unsigned int currentMS, Game &context);
	void handleSDLEvent(SDL_Event const &sdlEvent, Game &context);
private:
	//TTF_Font * textFont;	// SDL type for True-Type font rendering
	//Label * titleLabel;
	//Label * instruction1;
	//Label * instruction2;
	//Label * instruction3;
};

#endif