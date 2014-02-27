#ifndef DEFERRED_RENDERER
#define DEFERRED_RENDERER

#include "Renderer.h"
#include "Scene.h"

class DeferredRenderer : public Renderer
{
public:

	DeferredRenderer();
	~DeferredRenderer();

	virtual void init();
	
	virtual void update();
	virtual void renderScene();

protected:

	ModelLoader::GenericModel *dirLightQuad, *pointLightSphere, *spotLightCone;
	ShaderLoader::VertFragShader *dirLightShader, *pointLightShader, *spotLightShader, *stencilPassShader;
	
	GeometryBuffer gBuffer;

	virtual void geometryPass();
	virtual void stencilPass();
	virtual void dirLightPass();
	virtual void pointLightPass();
	virtual void spotLightPass();
	virtual void skyboxPass();
	virtual void finalPass();
	virtual void debugDrawBuffers();
};

#endif