#ifndef FRAMEBUFFER_CLASS
#define FRAMEBUFFER_CLASS

#include <GL/glew.h>

class Framebuffer
{
public:
	
	virtual void bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	}
	virtual void bindForReading()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
	}
	virtual void bindForWriting()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	}

	virtual void init() = 0;
	virtual void reload() = 0;
	virtual void initFrame() = 0;

protected:
	GLuint FBO;

};

#endif