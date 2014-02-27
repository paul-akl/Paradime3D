#include "CurrentUpdateState.h"
#include "OS_Layer.h"
#include "ShadowMapDefRenderer.h"

ShadowMapDefRenderer::ShadowMapDefRenderer()
{
	Message::show(MSG_INFO, MSG_RENDERER, "Deferred renderer (with shadow mapping) handle has been created.");
}
ShadowMapDefRenderer::~ShadowMapDefRenderer()
{

}

void ShadowMapDefRenderer::init()
{
	gBuffer.init();
	shadowMapBuffer.init();

	try
	{
		dirLightQuad = new ModelLoader::GenericModel(Config::renderer::dir_light_quad);
		pointLightSphere = new ModelLoader::GenericModel(Config::renderer::point_light_sphere);
		
		dirLightShader		= ShaderLoader::load(Config::renderer::dir_basic_shadowmap_vert_shader, Config::renderer::dir_basic_shadowmap_frag_shader);
		pointLightShader	= ShaderLoader::load(Config::renderer::point_light_vert_shader, Config::renderer::point_light_frag_shader);
		spotLightShader		= ShaderLoader::load(Config::renderer::spot_light_vert_shader, Config::renderer::spot_light_frag_shader);
		stencilPassShader	= ShaderLoader::load(Config::renderer::stencil_pass_vert_shader, Config::renderer::stencil_pass_frag_shader);
		dirShadowMapShader	= ShaderLoader::load(Config::renderer::dir_shadowmap_vert_shader, Config::renderer::dir_shadowmap_frag_shader);

	}
	catch(Message::messageCode error_arg)
	{
		error_arg.display();
	}

	dirLightShader->updateTextureUniforms();
	pointLightShader->updateTextureUniforms();
	spotLightShader->updateTextureUniforms();

	Message::show(MSG_INFO, MSG_RENDERER, "Deferred renderer (with shadow mapping) has been initialized.");
}
void ShadowMapDefRenderer::renderScene()
{
	//if(Config::drawDebugBuffers)
		dirShadowMapPass();

	gBuffer.initFrame();

	geometryPass();

	glEnable(GL_STENCIL_TEST);

	for(int i=0; i < Current::scene->lighting->pointLightPoolSize; i++)
	{
		Current::scene->lighting->currentPointLight = Current::scene->lighting->pointLightPool[i];

		stencilPass();
		pointLightPass();
	}

	glDisable(GL_STENCIL_TEST);

	dirLightPass();

	skyboxPass();

	finalPass();
}

void ShadowMapDefRenderer::dirLightPass()
{
	//gBuffer.bind();
	gBuffer.bindForWriting();
	//gBuffer.bindForReading();
	//glDrawBuffer(GL_COLOR_ATTACHMENT4);
	//shadowMapBuffer.initDirShadowMapReadPass();
	//gBuffer.initLightPass();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	//dirLightShader->updateFrame();
	dirLightQuad->render(dirLightShader);
	
	glDisable(GL_BLEND);
}
void ShadowMapDefRenderer::dirShadowMapPass()
{
	shadowMapBuffer.initDirShadowMapWritePass();
	dirShadowMapShader->updateFrame();
	
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	OS::resetFaceCulling();
    glDepthFunc(GL_LESS);

	Current::scene->renderObjects(dirShadowMapShader);
	
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	//glDepthMask(GL_FALSE);
}

void ShadowMapDefRenderer::finalPass()
{
	gBuffer.initFinalPass();
	//shadowMapBuffer.initFrame();
	glBlitFramebuffer(	0, 0, Config::currentScreenWidth, Config::currentScreenHeight,
						0, 0, Config::currentScreenWidth, Config::currentScreenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	//glBlitFramebuffer(	0, 0, 600, 600,
	//					100, 100, 400, 400, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	
	//shadowMapBuffer.initFrame();
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//shadowMapBuffer.bindForReading();
    //glReadBuffer(GL_DEPTH_COMPONENT);
	//glBlitFramebuffer(	0, 0, Config::currentScreenWidth, Config::currentScreenHeight,
	//					0, 0, Config::currentScreenWidth, Config::currentScreenHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

// FX Composer
// NVidia Shader Debugger