#ifndef SHADOWMAP_BUFFER
#define SHADOWMAP_BUFFER

#include "Framebuffer.h"

class ShadowMapBuffer : public Framebuffer
{
public:
	ShadowMapBuffer();
	~ShadowMapBuffer();

	void init();
	void reload();
	void initFrame();

	void initDirShadowMapWritePass();
	void initDirShadowMapReadPass();

protected:
	GLuint shadowMapTexture;
	GLuint diffuseTexture;

};

#endif