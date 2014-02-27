#ifndef SHADOW_MAP_DEF_RENDERER
#define SHADOW_MAP_DEF_RENDERER

#include "DeferredRenderer.h"
#include "ShadowMapBuffer.h"

class ShadowMapDefRenderer : public DeferredRenderer
{
public:
	ShadowMapDefRenderer();
	~ShadowMapDefRenderer();

	virtual void init();
	virtual void renderScene();

protected:
	ShadowMapBuffer shadowMapBuffer;
	ShaderLoader::VertFragShader *dirShadowMapShader;

	virtual void dirLightPass();
	virtual void dirShadowMapPass();
	virtual void finalPass();
};


#endif