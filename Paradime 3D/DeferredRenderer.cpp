#include "CurrentUpdateState.h"
#include "DeferredRenderer.h"
#include "OS_Layer.h"

DeferredRenderer::DeferredRenderer()
{
	Message::show(MSG_INFO, MSG_RENDERER, "Deferred renderer handle has been created.");
}
DeferredRenderer::~DeferredRenderer()
{

}

void DeferredRenderer::init()
{
	gBuffer.init();

	try
	{
		dirLightQuad = new ModelLoader::GenericModel(Config::renderer::dir_light_quad);
		pointLightSphere = new ModelLoader::GenericModel(Config::renderer::point_light_sphere);
		spotLightCone = new ModelLoader::GenericModel(Config::renderer::point_light_sphere);

		dirLightShader		= ShaderLoader::load(Config::renderer::dir_light_vert_shader, Config::renderer::dir_light_frag_shader);
		pointLightShader	= ShaderLoader::load(Config::renderer::point_light_vert_shader, Config::renderer::point_light_frag_shader);
		spotLightShader		= ShaderLoader::load(Config::renderer::spot_light_vert_shader, Config::renderer::spot_light_frag_shader);
		stencilPassShader	= ShaderLoader::load(Config::renderer::stencil_pass_vert_shader, Config::renderer::stencil_pass_frag_shader);
	}
	catch(Message::messageCode error_arg)
	{
		error_arg.display();
	}

	dirLightShader->updateTextureUniforms();
	pointLightShader->updateTextureUniforms();
	spotLightShader->updateTextureUniforms();

	Message::show(MSG_INFO, MSG_RENDERER, "Deferred renderer has been initialized.");
}

void DeferredRenderer::update()
{
	if(Config::resizeWindow)
	{
		gBuffer.reload();
		Config::resizeWindow = false;
	}
}
void DeferredRenderer::renderScene()
{
	gBuffer.initFrame();

	geometryPass();

	glEnable(GL_STENCIL_TEST);

	for(int i=0; i < Current::scene->lighting->pointLightPoolSize; i++)
	{
		Current::scene->lighting->currentLight = Current::scene->lighting->pointLightPool[i];
		Current::scene->lighting->currentPointLight = Current::scene->lighting->pointLightPool[i];

		stencilPass();
		pointLightPass();
	}

	for(int i=0; i < Current::scene->lighting->spotLightPoolSize; i++)
	{
		Current::scene->lighting->currentLight = Current::scene->lighting->spotLightPool[i];
		Current::scene->lighting->currentSpotLight = Current::scene->lighting->spotLightPool[i];

		stencilPass();
		spotLightPass();
	}

	glDisable(GL_STENCIL_TEST);
	
	if(Config::drawDebugBuffers)
		dirLightPass();

	skyboxPass();

	//if(Config::drawDebugBuffers)
	//	gBuffer.drawBuffers();
		//debugDrawBuffers();
	//else
		finalPass();
}

void DeferredRenderer::geometryPass()
{
	gBuffer.initGeometryPass();

	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	OS::resetFaceCulling();
	
	Current::scene->renderObjects();

	glDepthMask(GL_FALSE);
}
void DeferredRenderer::stencilPass()
{
	gBuffer.initStencilPass();

	glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_ALWAYS, 0, 0);

	//glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR, GL_KEEP);
	//glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR, GL_KEEP);

	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	pointLightSphere->render(stencilPassShader);
}
void DeferredRenderer::dirLightPass() 
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	dirLightQuad->render(dirLightShader);
	
	glDisable(GL_BLEND);
}
void DeferredRenderer::pointLightPass()
{
	gBuffer.initLightPass();

	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
        
	glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

	pointLightSphere->render(pointLightShader);
	
    glCullFace(Config::engine::face_culling_mode);
    //glCullFace(GL_BACK);
	glDisable(GL_BLEND);
}
void DeferredRenderer::spotLightPass()
{
	gBuffer.initLightPass();

	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

	spotLightCone->render(spotLightShader);
	
    glCullFace(Config::engine::face_culling_mode);
    //glCullFace(GL_BACK);
	glDisable(GL_BLEND);
}
void DeferredRenderer::skyboxPass()
{
	glEnable(GL_DEPTH_TEST);
	Current::scene->renderSky();
}
void DeferredRenderer::finalPass()
{
	gBuffer.initFinalPass();
	glBlitFramebuffer(	0, 0, Config::currentScreenWidth, Config::currentScreenHeight,
						0, 0, Config::currentScreenWidth, Config::currentScreenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}
void DeferredRenderer::debugDrawBuffers()
{
	//gBuffer.drawBuffers();

	//gBuffer.initFinalPass();	
	//glBlitFramebuffer(	0, 0, Config::currentScreenWidth, Config::currentScreenHeight, 
	//					0, Config::currentScreenHeight / 2, Config::currentScreenWidth / 2, Config::currentScreenHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}