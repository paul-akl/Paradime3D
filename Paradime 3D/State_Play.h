#ifndef STATE_PLAY
#define STATE_PLAY

#include "CommonClasses.h"
#include "GameState.h"
#include "Label.h"
// C stdlib and C time libraries for rand and time functions
#include <cstdlib>
#include <ctime>
#include "Clock.h"
#include "Skybox.h"
#include "DeferredRenderer.h"

class StatePlay : public GameState
{
public:

	StatePlay(void);
	~StatePlay(void);
	void init(Game &context);
	void update(Game &context);
	void draw();
	void handleSDLEvent(SDL_Event const &sdlEvent, Game &context);

private:

	Common::InputState inputs;
	Common::Perspective perspective;
	DeferredRenderer renderer;

	// Another *strong* candidate for creating a new class type
	// is character/agent, to encapsulate player and target data,
	// potentially to include an Agent::draw() function and
	// perhaps also generalise collision detection

	float xpos;
	float ypos;
	float xsize;
	float ysize;

	float targetXPos;
	float targetYPos;
	float targetXSize;
	float targetYSize;

	clock_t lastTime; // clock_t is an integer type
	clock_t currentTime; // use this to track time between frames

	Label * playerLabel;
	Label * targetLabel;

	TTF_Font * textFont;	// SDL type for True-Type font rendering

};

#endif