#include "GeometryBuffer.h"

#include "Config.h"
#include "CurrentUpdateState.h"

#include <iostream>

GeometryBuffer::GeometryBuffer()
{
	FBO = 0;
	depthBuffer = 0;
	finalBuffer = 0;
	
	for(int i=0; i < (sizeof(buffers) / sizeof(*buffers)); i++)
		buffers[i] = 0;

	geometryDrawBuffers[0] = GL_COLOR_ATTACHMENT0;
	geometryDrawBuffers[1] = GL_COLOR_ATTACHMENT0 + 1;
	geometryDrawBuffers[2] = GL_COLOR_ATTACHMENT0 + 2;
	geometryDrawBuffers[3] = GL_COLOR_ATTACHMENT0 + 3;

	Message::show(MSG_INFO, MSG_GBUFFER, " handle has been created.");
}
GeometryBuffer::~GeometryBuffer()
{
	// Clean the existing buffers

    if(FBO != 0)
        glDeleteFramebuffers(1, &FBO);

    if(buffers[0] != 0)
        glDeleteTextures(sizeof(buffers) / sizeof(*buffers), buffers);

	if(depthBuffer != 0)
		glDeleteTextures(1, &depthBuffer);

	if(finalBuffer != 0)
		glDeleteTextures(1, &finalBuffer);
}

void GeometryBuffer::init()
{
	// Create the FBO
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);

	// Create geometry pass buffers
	glGenTextures(sizeof(buffers) / sizeof(*buffers), buffers);
	glGenTextures(1, &depthBuffer);
	glGenTextures(1, &finalBuffer);

	// Create textures
	for(int i=0; i < (sizeof(buffers) / sizeof(*buffers)); i++)
	{
		glBindTexture(GL_TEXTURE_2D, buffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, Config::currentScreenWidth, Config::currentScreenHeight, 0, GL_RGB, GL_FLOAT, NULL);

		// Using nearest filtering instead of linear, to avoid any distortions
		// and unnecessary interpolation (since the sampling is 1 to 1)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, buffers[i], 0);
	}
	
	// Create depth buffer
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, Config::currentScreenWidth, Config::currentScreenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

	// Create the final buffer, that gets renderred to the screen
	glBindTexture(GL_TEXTURE_2D, finalBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Config::currentScreenWidth, Config::currentScreenHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, finalBuffer, 0);

	// Check for errors and throw an exception in case of one
	GLenum bufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(bufferStatus != GL_FRAMEBUFFER_COMPLETE)
		throw Message::messageCode(MSG_FATAL_ERROR, MSG_GBUFFER, "Geometry buffer has failed to load: " + bufferStatus);

	// Restore the default FBO, so it doesn't get changed from the outside of the class
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	Current::positionMap = GBufferPosition;
	Current::diffuseMap = GBufferDiffuse;
	Current::normalMap = GBufferNormal;

	Message::show(MSG_INFO, MSG_GBUFFER, "  has been initialized.");
}
void GeometryBuffer::initStencilShaders(std::string stencilVertShaderName_arg, std::string stencilFragShaderName_arg)
{
	try
	{
		//stencilPassShader = ShaderLoader::load(stencilVertShaderName_arg, stencilFragShaderName_arg);
	}
	catch(Message::messageCode error_arg)
	{
		error_arg.display();
	}
}

void GeometryBuffer::updatePerFrame()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT4);
    glClear(GL_COLOR_BUFFER_BIT);
}
void GeometryBuffer::initGeometryPass()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);

    glDrawBuffers(sizeof(geometryDrawBuffers) / sizeof(*geometryDrawBuffers), geometryDrawBuffers);

	/*GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, 
						     GL_COLOR_ATTACHMENT1,
						     GL_COLOR_ATTACHMENT2,
							 GL_COLOR_ATTACHMENT3	};

    //glDrawBuffers(sizeof(DrawBuffers) / sizeof(*DrawBuffers), DrawBuffers);*/
}
void GeometryBuffer::initStencilPass()
{
	// Disable the draw buffers
	glDrawBuffer(GL_NONE);
}
void GeometryBuffer::initLightPass()
{
	glDrawBuffer(GL_COLOR_ATTACHMENT4);
	
	for(int i=0; i < (sizeof(buffers) / sizeof(*buffers)); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);		
		glBindTexture(GL_TEXTURE_2D, buffers[GBufferPosition + i]);
	}
	//glActiveTexture(GL_TEXTURE0);
}
void GeometryBuffer::initFinalPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
    glReadBuffer(GL_COLOR_ATTACHMENT4);
}

void GeometryBuffer::drawBuffers()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glReadBuffer(GL_COLOR_ATTACHMENT0);
	//glBlitFramebuffer(0, 0, Config::currentScreenWidth, Config::currentScreenHeight, 0, Config::currentScreenHeight / 2, Config::currentScreenWidth / 2, Config::currentScreenHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBlitFramebuffer(	0, 0, Config::currentScreenWidth, Config::currentScreenHeight, 0, 0, Config::currentScreenWidth, Config::currentScreenHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    
	//glReadBuffer(GL_COLOR_ATTACHMENT1);
	//glBlitFramebuffer(0, 0, Config::currentScreenWidth, Config::currentScreenHeight, Config::currentScreenWidth / 2, Config::currentScreenHeight / 2, Config::currentScreenWidth, Config::currentScreenHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    
	//glReadBuffer(GL_COLOR_ATTACHMENT2);
	//glBlitFramebuffer(0, 0, Config::currentScreenWidth, Config::currentScreenHeight, 0, 0, Config::currentScreenWidth / 2, Config::currentScreenHeight / 2, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	
    //glReadBuffer(GL_COLOR_ATTACHMENT3);
	//glBlitFramebuffer(0, 0, Config::currentScreenWidth, Config::currentScreenHeight, Config::currentScreenWidth / 2, 0, Config::currentScreenWidth, Config::currentScreenHeight / 2, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}