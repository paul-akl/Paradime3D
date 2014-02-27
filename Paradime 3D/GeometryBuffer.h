#ifndef GEOMETRY_BUFFER
#define GEOMETRY_BUFFER

#include "Framebuffer.h"
#include "ErrorHandler.h"
#include "Loader_Shader.h"

class GeometryBuffer : public Framebuffer
{
public:

	enum GBufferTextureType 
	{
		GBufferPosition,
		GBufferDiffuse,
		GBufferNormal,
		GBufferTexCoord,
		GBufferNumTextures
    };

	GeometryBuffer();
	~GeometryBuffer();
	
	virtual void init();
	void reload();
	void initFrame();

	void initGeometryPass();
	void initStencilPass();

	void initLightPass();
	void initFinalPass();

	void drawBuffers();

protected:

	GLuint	depthBuffer,
			finalBuffer,
			buffers[GBufferNumTextures];

	GLenum	geometryDrawBuffers[4];

};

#endif