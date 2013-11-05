#ifndef DEFERRED_RENDERER
#define DEFERRED_RENDERER

#include "Scene.h"

class DeferredRenderer
{
public:

	DeferredRenderer();
	~DeferredRenderer();

	void init();

	void renderScene();

private:

	ModelLoader::GenericModel *dirLightQuad, *pointLightSphere;
	ShaderLoader::Shader *dirLightShader, *pointLightShader, *spotLightShader, *stencilPassShader;
	
	GeometryBuffer gBuffer;

	void geometryPass();
	void stencilPass();
	void dirLightPass();
	void pointLightPass();
	void spotLightPass();
	void skyboxPass();
	void finalPass();
	void debugDrawBuffers();
};

#endif