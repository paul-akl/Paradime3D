#ifndef GEOMETRY_BUFFER
#define GEOMETRY_BUFFER

#include "ErrorHandler.h"
#include "Loader_Shader.h"

#define numGBufferTextures	

class GeometryBuffer
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

	void init();
	void initStencilShaders(std::string stencilVertShaderName_arg, std::string stencilFragShaderName_arg);

	void updatePerFrame();
	void initGeometryPass();
	void initStencilPass();

	void initLightPass();
	void initFinalPass();

	void drawBuffers();

private:
	GLuint	FBO,
			depthBuffer,
			finalBuffer,
			buffers[GBufferNumTextures];

	GLenum	geometryDrawBuffers[4];

};

#endif