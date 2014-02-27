
#include "Config.h"
#include "CurrentUpdateState.h"
#include "ShadowMapBuffer.h"

ShadowMapBuffer::ShadowMapBuffer()
{

}
ShadowMapBuffer::~ShadowMapBuffer()
{

}

void ShadowMapBuffer::init()
{
	// Create the FBO
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);

	// Create the depth texture
	glGenTextures(1, &shadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, Config::graphics::dir_shadow_res_x, Config::graphics::dir_shadow_res_y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Set the depth texture as depth buffer in the FBO
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMapTexture, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexture, 0);
	
	glGenTextures(1, &diffuseTexture);

	glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, Config::currentScreenWidth, Config::currentScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, Config::graphics::dir_shadow_res_x, Config::graphics::dir_shadow_res_y, 0, GL_RGBA, GL_FLOAT, NULL);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, diffuseTexture, 0);

	//glDrawBuffer(GL_NONE); // No color buffer is drawn to
	//glReadBuffer(GL_NONE); // No color buffer is read from

	Current::dirShadowMap = shadowMapTexture;
}
void ShadowMapBuffer::reload()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, Config::graphics::dir_shadow_res_x, Config::graphics::dir_shadow_res_y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
}
void ShadowMapBuffer::initFrame()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
}

void ShadowMapBuffer::initDirShadowMapWritePass()
{
	Current::diffuseMap = diffuseTexture;
	glActiveTexture(GL_TEXTURE0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}
void ShadowMapBuffer::initDirShadowMapReadPass()
{
    glActiveTexture(GL_COLOR_ATTACHMENT0);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
}


