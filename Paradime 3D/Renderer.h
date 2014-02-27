#ifndef BASE_RENDERER_CLASS
#define BASE_RENDERER_CLASS

class Renderer
{
public:

	virtual void init()			= 0;
	virtual void update()		= 0;
	virtual void renderScene()	= 0;
};

#endif