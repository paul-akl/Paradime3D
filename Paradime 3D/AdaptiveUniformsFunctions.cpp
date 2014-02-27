#include <GL\glew.h>

#include "AdaptiveUniformsFunctions.h"
#include "Config.h"
#include "CurrentUpdateState.h"

namespace UniformFunctions
{
	modelMatUniform::modelMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "Model";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
		//std::cout << std::endl << "Model Uniform Function created:" << uniformID << " | " << uniformName << std::endl << std::endl;
	}
	void modelMatUniform::updateUniform()
	{
		Current::objectParameters->calcModelMat();
		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &Current::objectParameters->modelMat.m[0]);
	}
	viewMatUniform::viewMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "View";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void viewMatUniform::updateUniform()
	{
		//if(Current::player->currentCamera->modified)
		//{
			glUniformMatrix4fv(uniformID, 1, GL_FALSE, &Current::player->currentCamera->viewMatrix.m[0]);
		//}
	}
	projectionMatUniform::projectionMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "Projection";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void projectionMatUniform::updateUniform()
	{
		if(Current::perspective->modified)
		{
			Current::perspective->modified = false;
			Current::perspective->perspectiveMat.perspective(Config::graphics::FOV, 
															 Config::currentScreenWidth, Config::currentScreenHeight, 
															 Config::engine::z_near, Config::engine::z_far);
		}
		//Current::perspective->perspectiveMat.identity();
		//Current::perspective->perspectiveMat.ortho(-400, 400, -400, 400, 0, 1000);
		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &Current::perspective->perspectiveMat.m[0]);
	}
	viewProjectionMatUniform::viewProjectionMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "VP";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void viewProjectionMatUniform::updateUniform()
	{
		//if(Current::perspective->modified)
		//{
		//	Current::perspective->modified == false;
			Current::perspective->perspectiveMat.perspective(Config::graphics::FOV, 
															 Config::currentScreenWidth, Config::currentScreenHeight, 
															 Config::engine::z_near, Config::engine::z_far);
		//}

		Math3d::Mat4f viewProjMat = Current::perspective->perspectiveMat * Current::player->currentCamera->viewMatrix;

		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &viewProjMat.m[0]);
	}
	modelViewMatUniform::modelViewMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "MV";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void modelViewMatUniform::updateUniform()
	{
		if(Current::objectParameters->modified)
		{
			Current::objectParameters->modified = false;
			Current::objectParameters->modelMat.identity();
			Current::objectParameters->modelMat.rotate(Current::objectParameters->rotationVec);
			Current::objectParameters->modelMat.transform(Current::objectParameters->positionVec);
			Current::objectParameters->modelMat.scale(Current::objectParameters->scaleVec);
		}
		
		if(Current::player->currentCamera->modified)
		{
			Current::player->currentCamera->modified = false;
			Current::player->currentCamera->updateViewMatrix();
		}

		Current::objectParameters->modelViewMat = Current::player->currentCamera->viewMatrix * Current::objectParameters->modelMat;
		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &Current::objectParameters->modelViewMat.m[0]);
	}
	modelViewProjectionMatUniform::modelViewProjectionMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "MVP";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void modelViewProjectionMatUniform::updateUniform()
	{
		if(Current::objectParameters->modified)
		{
			Current::objectParameters->modified = false;
			Current::objectParameters->modelMat.identity();
			Current::objectParameters->modelMat.rotate(Current::objectParameters->rotationVec);
			Current::objectParameters->modelMat.transform(Current::objectParameters->positionVec);
			Current::objectParameters->modelMat.scale(Current::objectParameters->scaleVec);
		}

		if(Current::perspective->modified)
		{
			Current::perspective->modified = false;
			Current::perspective->perspectiveMat.perspective(Config::graphics::FOV, 
															 Config::currentScreenWidth, Config::currentScreenHeight, 
															 Config::engine::z_near, Config::engine::z_far);
		}
		//Current::perspective->perspectiveMat.identity();
		//Current::perspective->perspectiveMat.ortho(-50, 50, -50, 50, 100, 200);
		//Current::perspective->perspectiveMat.ortho(-Config::currentScreenWidth/20, Config::currentScreenWidth/20, -Config::currentScreenHeight/20, Config::currentScreenHeight/20, 100, 300);
		Current::objectParameters->modelViewProjMat = Current::perspective->perspectiveMat * Current::player->currentCamera->viewMatrix * Current::objectParameters->modelMat;
		
		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &Current::objectParameters->modelViewProjMat.m[0]);
	}
	screenSizeUniform::screenSizeUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "screenSize";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void screenSizeUniform::updateUniform()
	{
		glUniform2f(uniformID, (float)Config::currentScreenWidth, (float)Config::currentScreenHeight);
	}
	numPointLightsUniform::numPointLightsUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "numPointLights";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void numPointLightsUniform::updateUniform()
	{
		glUniform1i(uniformID, Current::lighting->pointLightPoolSize);
	}
	numSpotLightsUniform::numSpotLightsUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "numSpotLights";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void numSpotLightsUniform::updateUniform()
	{

	}
	pointLightViewProjectionMatUniform::pointLightViewProjectionMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "pointLightMVP";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void pointLightViewProjectionMatUniform::updateUniform()
	{
		//if(Current::scene->lighting->currentPointLight->modified)
		//{
			Current::scene->lighting->currentLight->updateLightModelMat();

		//}

		//if(Current::perspective->modified)
		//{
		//	Current::perspective->modified == false;
			Current::perspective->perspectiveMat.perspective(Config::graphics::FOV, 
															 Config::currentScreenWidth, Config::currentScreenHeight, 
															 Config::engine::z_near, Config::engine::z_far);
		//}
		Math3d::Mat4f lightViewProj = Current::perspective->perspectiveMat * Current::player->currentCamera->viewMatrix * Current::scene->lighting->currentLight->lightModelMat;

		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &lightViewProj.m[0]);
	}
	spotLightViewProjectionMatUniform::spotLightViewProjectionMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "spotLightMVP";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void spotLightViewProjectionMatUniform::updateUniform()
	{
		//if(Current::scene->lighting->currentPointLight->modified)
		//{
			//Current::scene->lighting->currentSpotLight->calculateModelScale();

			//Current::scene->lighting->currentSpotLight->lightModelMat.identity();
			//Current::scene->lighting->currentSpotLight->lightModelMat.transform(Current::scene->lighting->currentSpotLight->position);
			//Current::scene->lighting->currentSpotLight->lightModelMat.transform(Current::scene->player.position);
			//Current::scene->lighting->currentSpotLight->lightModelMat.scale(Current::scene->lighting->currentSpotLight->modelScale);
			//Current::scene->lighting->currentPointLight->lightSphereMat.scale(1);
			Math3d::Mat4f lightMat;
			lightMat.transform(Current::scene->player.position);
			lightMat.scale(50.0f);
		//}

		//if(Current::perspective->modified)
		//{
		//	Current::perspective->modified == false;
			Current::perspective->perspectiveMat.perspective(Config::graphics::FOV, 
															 Config::currentScreenWidth, Config::currentScreenHeight, 
															 Config::engine::z_near, Config::engine::z_far);
		//}
		Current::scene->lighting->currentLight->updateLightModelMat();
		Math3d::Mat4f lightViewProj = Current::perspective->perspectiveMat * Current::player->currentCamera->viewMatrix * Current::scene->lighting->currentLight->lightModelMat;

		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &lightViewProj.m[0]);
	}	
	stencilPassViewProjectionMatUniform::stencilPassViewProjectionMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "stencilMVP";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void stencilPassViewProjectionMatUniform::updateUniform()
	{
		//if(Current::perspective->modified)
		//{
		//	Current::perspective->modified == false;
			Current::perspective->perspectiveMat.perspective(Config::graphics::FOV, 
															 Config::currentScreenWidth, Config::currentScreenHeight, 
															 Config::engine::z_near, Config::engine::z_far);
		//}

		Current::scene->lighting->currentLight->updateLightModelMat();

		Math3d::Mat4f stencilViewProj = Current::perspective->perspectiveMat * Current::player->currentCamera->viewMatrix * Current::scene->lighting->currentLight->lightModelMat;

		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &stencilViewProj.m[0]);
	}
	
	dirLightMatUniform::dirLightMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "dirLightMat";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void dirLightMatUniform::updateUniform()
	{
		Math3d::Mat4f lightMat;

		lightMat.identity();
		lightMat.rotate(Math3d::Vec3f(Config::renderer::dir_light_quad_rotation_x, Config::renderer::dir_light_quad_rotation_y, Config::renderer::dir_light_quad_rotation_z));
		lightMat.transform(Math3d::Vec3f(Config::renderer::dir_light_quad_offset_x, Config::renderer::dir_light_quad_offset_y, Config::renderer::dir_light_quad_offset_z));

		//Math3d::Mat4f viewProjMat = lightMat;

		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &lightMat.m[0]);
	}
	dirLightProjectionMatUniform::dirLightProjectionMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "dirLightP";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void dirLightProjectionMatUniform::updateUniform()
	{
		Math3d::Mat4f lightMat;

		lightMat.identity();
		lightMat.rotate(Math3d::Vec3f(Config::renderer::dir_light_quad_rotation_x, Config::renderer::dir_light_quad_rotation_y, Config::renderer::dir_light_quad_rotation_z));
		lightMat.transform(Math3d::Vec3f(Config::renderer::dir_light_quad_offset_x, Config::renderer::dir_light_quad_offset_y, Config::renderer::dir_light_quad_offset_z));

		if(Current::perspective->modified)
		{
			Current::perspective->modified = false;
			Current::perspective->perspectiveMat.perspective(Config::graphics::FOV, 
															 Config::currentScreenWidth, Config::currentScreenHeight, 
															 Config::engine::z_near, Config::engine::z_far);
		}

		Math3d::Mat4f viewProjMat = Current::perspective->perspectiveMat * lightMat;

		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &viewProjMat.m[0]);
	}
	directionalLightBaseColorUniform::directionalLightBaseColorUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "directionalLight.base.color";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void directionalLightBaseColorUniform::updateUniform()
	{
		//if(Current::scene->lighting->directionalLight->colorModified)
		//{
			glUniform3f(uniformID, Current::scene->lighting->directionalLight->color.x, Current::scene->lighting->directionalLight->color.y, Current::scene->lighting->directionalLight->color.z);
			Current::scene->lighting->directionalLight->colorModified = false;
		//}
	}
	directionalLightBaseAmbientIntensityUniform::directionalLightBaseAmbientIntensityUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "directionalLight.base.ambientIntensity";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void directionalLightBaseAmbientIntensityUniform::updateUniform()
	{
		//if(Current::scene->lighting->directionalLight->ambientIntensityModified)
		//{
			glUniform1f(uniformID, Current::scene->lighting->directionalLight->ambientIntensity);
			Current::scene->lighting->directionalLight->ambientIntensityModified = false;
		//}
	}
	directionalLightDirectionUniform::directionalLightDirectionUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "directionalLight.direction";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void directionalLightDirectionUniform::updateUniform()
	{
		//if(Current::scene->lighting->directionalLight->directionUpdated)
			Current::scene->lighting->directionalLight->calcRotationVec();

		glUniform3f(uniformID,	Current::scene->lighting->directionalLight->rotationVec.x, 
								Current::scene->lighting->directionalLight->rotationVec.y, 
								Current::scene->lighting->directionalLight->rotationVec.z	);
	}
	directionalLightDiffuseIntensityUniform::directionalLightDiffuseIntensityUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "directionalLight.base.diffuseIntensity";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void directionalLightDiffuseIntensityUniform::updateUniform()
	{
		//if(Current::scene->lighting->directionalLight->diffuseIntensityModified)
		//{
			glUniform1f(uniformID, Current::scene->lighting->directionalLight->diffuseIntensity);
			Current::scene->lighting->directionalLight->diffuseIntensityModified = false;
		//}
	}
	
	pointLightBaseColorUniform::pointLightBaseColorUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "pointLight.base.color";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void pointLightBaseColorUniform::updateUniform()
	{
		glUniform3f(uniformID, Current::scene->lighting->currentPointLight->color.x, Current::scene->lighting->currentPointLight->color.y, Current::scene->lighting->currentPointLight->color.z);
	}
	pointLightAmbientIntensityUniform::pointLightAmbientIntensityUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "pointLight.base.ambientIntensity";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void pointLightAmbientIntensityUniform::updateUniform()
	{
		glUniform1f(uniformID, Current::scene->lighting->currentPointLight->ambientIntensity);
	}
	pointLightPositionUniform::pointLightPositionUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "pointLight.position";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void pointLightPositionUniform::updateUniform()
	{
		glUniform3f(uniformID, Current::scene->lighting->currentPointLight->position.x, Current::scene->lighting->currentPointLight->position.y, Current::scene->lighting->currentPointLight->position.z);
	}
	pointLightBaseDiffuseIntensityUniform::pointLightBaseDiffuseIntensityUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "pointLight.base.diffuseIntensity";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void pointLightBaseDiffuseIntensityUniform::updateUniform()
	{
		glUniform1f(uniformID, Current::scene->lighting->currentPointLight->diffuseIntensity);
	}
	pointLightAttenuationConstantUniform::pointLightAttenuationConstantUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "pointLight.atten.constant";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void pointLightAttenuationConstantUniform::updateUniform()
	{
		glUniform1f(uniformID, Current::scene->lighting->currentPointLight->attenuation.constant);
	}
	pointLightAttenuationLinearUniform::pointLightAttenuationLinearUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "pointLight.atten.linear";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void pointLightAttenuationLinearUniform::updateUniform()
	{
		glUniform1f(uniformID, Current::scene->lighting->currentPointLight->attenuation.linear);
	}
	pointLightAttenuationExponentialUniform::pointLightAttenuationExponentialUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "pointLight.atten.exp";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void pointLightAttenuationExponentialUniform::updateUniform()
	{
		glUniform1f(uniformID, Current::scene->lighting->currentPointLight->attenuation.exp);
	}

	spotLightBaseColorUniform::spotLightBaseColorUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "spotLight.base.color";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void spotLightBaseColorUniform::updateUniform()
	{
		glUniform3f(uniformID, Current::scene->lighting->currentSpotLight->color.x, Current::scene->lighting->currentSpotLight->color.y, Current::scene->lighting->currentSpotLight->color.z);
	}
	spotLightPositionUniform::spotLightPositionUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "spotLight.position";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void spotLightPositionUniform::updateUniform()
	{
		glUniform3f(uniformID, Current::scene->lighting->currentSpotLight->position.x, Current::scene->lighting->currentSpotLight->position.y, Current::scene->lighting->currentSpotLight->position.z);
	}
	spotLightAmbientIntensityUniform::spotLightAmbientIntensityUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "spotLight.base.ambientIntensity";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void spotLightAmbientIntensityUniform::updateUniform()
	{
		glUniform1f(uniformID, Current::scene->lighting->currentSpotLight->ambientIntensity);
	}
	spotLightBaseDiffuseIntensityUniform::spotLightBaseDiffuseIntensityUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "spotLight.base.diffuseIntensity";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void spotLightBaseDiffuseIntensityUniform::updateUniform()
	{
		glUniform1f(uniformID, Current::scene->lighting->currentSpotLight->diffuseIntensity);
	}
	spotLightAttenuationConstantUniform::spotLightAttenuationConstantUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "spotLight.atten.constant";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void spotLightAttenuationConstantUniform::updateUniform()
	{
		glUniform1f(uniformID, Current::scene->lighting->currentSpotLight->attenuation.constant);
	}
	spotLightAttenuationLinearUniform::spotLightAttenuationLinearUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "spotLight.atten.linear";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void spotLightAttenuationLinearUniform::updateUniform()
	{
		glUniform1f(uniformID, Current::scene->lighting->currentSpotLight->attenuation.linear);
	}
	spotLightAttenuationExponentialUniform::spotLightAttenuationExponentialUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "spotLight.atten.exp";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void spotLightAttenuationExponentialUniform::updateUniform()
	{
		glUniform1f(uniformID, Current::scene->lighting->currentSpotLight->attenuation.exp);
	}
	spotLightDirectionUniform::spotLightDirectionUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "spotLight.direction";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void spotLightDirectionUniform::updateUniform()
	{
		//glUniform3f(uniformID, Current::scene->lighting->currentSpotLight->direction.x, Current::scene->lighting->currentSpotLight->direction.y, Current::scene->lighting->currentSpotLight->direction.z);
		Current::scene->lighting->currentSpotLight->rotationVec = Current::scene->player.targetVector;
		Current::scene->lighting->currentSpotLight->position = Current::scene->player.position;

		//Current::scene->lighting->currentSpotLight->calcRotationVec();
		glUniform3f(uniformID,	Current::scene->lighting->currentSpotLight->rotationVec.x, 
								Current::scene->lighting->currentSpotLight->rotationVec.y, 
								Current::scene->lighting->currentSpotLight->rotationVec.z	);
	}
	spotLightCutoffUniform::spotLightCutoffUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "spotLight.cutoff";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void spotLightCutoffUniform::updateUniform()
	{
		glUniform1f(uniformID, cosf(Math3d::toRadian(Current::scene->lighting->currentSpotLight->cutoff)));
	}

	dirShadowMapMVPUniform::dirShadowMapMVPUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "dirShadowMapMVP";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void dirShadowMapMVPUniform::updateUniform()
	{		
		//if(Current::scene->lighting->directionalLight->MVPUpdated)
			//Current::scene->lighting->directionalLight->calcShadowMapMat();
			
		if(Current::objectParameters->modified)
		{
			Current::objectParameters->modified = false;
			Current::objectParameters->modelMat.identity();
			Current::objectParameters->modelMat.rotate(Current::objectParameters->rotationVec);
			Current::objectParameters->modelMat.transform(Current::objectParameters->positionVec);
			Current::objectParameters->modelMat.scale(Current::objectParameters->scaleVec);
		}
		if(Current::perspective->modified)
		{
			Current::perspective->modified = false;
			Current::perspective->perspectiveMat.perspective(Config::graphics::FOV, 
															 Config::currentScreenWidth, Config::currentScreenHeight, 
															 Config::engine::z_near, Config::engine::z_far);
		}

		Math3d::Mat4f view, proj;
		Math3d::Vec3f pos = Math3d::Vec3f(-25.0, -25.0, -25.0);
		pos.rotate(Current::scene->lighting->directionalLight->direction.z, Math3d::Vec3f(0.0, 0.0, 1.0));
		pos.rotate(Current::scene->lighting->directionalLight->direction.x, Math3d::Vec3f(1.0, 0.0, 0.0));
		pos.rotate(Current::scene->lighting->directionalLight->direction.y, Math3d::Vec3f(0.0, 1.0, 0.0));

		//view.identity();
		//proj.identity();

		//view.initCamera(pos, Math3d::Vec3f(0.0, 0.0, 0.0), Math3d::Vec3f(0.0, 1.0, 0.0));
		view.initCamera(pos + Current::player->position, Current::player->position, Math3d::Vec3f(0.0f, 1.0f, 0.0f));
		proj.ortho(-10, 10, -10, 10, -10, 20);
		//proj = Current::perspective->perspectiveMat;

		//proj.perspective(Config::graphics::FOV, 
		//				 Config::graphics::dir_shadow_res_x, Config::graphics::dir_shadow_res_y, 
		//				 Config::engine::z_near, Config::engine::z_far);

		Current::scene->lighting->directionalLight->MVP = proj * view * Current::objectParameters->modelMat;
		
		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &Current::scene->lighting->directionalLight->MVP.m[0]);
	}
	dirShadowMapBiasMVPUniform::dirShadowMapBiasMVPUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "dirShadowMapBiasMVP";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void dirShadowMapBiasMVPUniform::updateUniform()
	{		
		//if(Current::scene->lighting->directionalLight->MVPUpdated)
			Current::scene->lighting->directionalLight->calcShadowMapMat();
		
		Math3d::Mat4f biasMVP = Current::scene->lighting->directionalLight->MVP * Current::scene->lighting->directionalLight->shadowMapBias;
		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &biasMVP.m[0]);
	}
	
	cameraPosUniform::cameraPosUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "cameraPos";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void cameraPosUniform::updateUniform()
	{
		glUniform3f(uniformID, Current::player->currentCamera->position.x, Current::player->currentCamera->position.y, Current::player->currentCamera->position.z);
	}
	cameraUpUniform::cameraUpUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "cameraUp";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void cameraUpUniform::updateUniform()
	{
		glUniform3f(uniformID, Current::player->currentCamera->upVector.x, Current::player->currentCamera->upVector.y, Current::player->currentCamera->upVector.z);
	}
	cameraRightUniform::cameraRightUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "cameraRight";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void cameraRightUniform::updateUniform()
	{
		glUniform3f(uniformID, Current::player->currentCamera->target.x, Current::player->currentCamera->target.y, Current::player->currentCamera->target.z);
	}

	positionTextureUniform::positionTextureUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "positionMap";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void positionTextureUniform::updateUniform()
	{
		glUniform1i(uniformID, Current::positionMap);
	}
	diffuseTextureUniform::diffuseTextureUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "diffuseMap";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void diffuseTextureUniform::updateUniform()
	{
		glUniform1i(uniformID, Current::diffuseMap);
	}
	normalTextureUniform::normalTextureUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "normalMap";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void normalTextureUniform::updateUniform()
	{
		glUniform1i(uniformID, Current::normalMap);
	}

	sunGlowTextureUniform::sunGlowTextureUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "sunGlowMap";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void sunGlowTextureUniform::updateUniform()
	{
		glUniform1i(uniformID, Current::sunGlowMap);
	}
	skyMapTextureUniform::skyMapTextureUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "skyMap";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void skyMapTextureUniform::updateUniform()
	{
		glUniform1i(uniformID, Current::skyMap);
	}
	dirShadowMapTextureUniform::dirShadowMapTextureUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "dirShadowMap";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void dirShadowMapTextureUniform::updateUniform()
	{
		glUniform1i(uniformID, Current::dirShadowMap);
	}

	fogDensityUniform::fogDensityUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "fogDensity";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void fogDensityUniform::updateUniform()
	{
		glUniform1f(uniformID, Config::graphics::fog_density);
	}
	fogColorUniform::fogColorUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "fogColor";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void fogColorUniform::updateUniform()
	{
		glUniform3f(uniformID, Config::graphics::fog_color_x, Config::graphics::fog_color_y, Config::graphics::fog_color_z);
	}

	test1PerModelMatUniform::test1PerModelMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "test1PerModelMat";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void test1PerModelMatUniform::updateUniform()
	{
		Math3d::Mat4f testMat, rot, trans, invTrans;
		Math3d::Vec3f pos, dir;

		//testMat.identity();
		//rot.identity();
		//trans.identity();
		
		
	
		pos = Math3d::Vec3f(-50.0, -50.0, -50.0);
		
		pos.rotate(Current::scene->lighting->directionalLight->direction.z, Math3d::Vec3f(0.0, 0.0, 1.0));
		pos.rotate(Current::scene->lighting->directionalLight->direction.x, Math3d::Vec3f(1.0, 0.0, 0.0));
		pos.rotate(Current::scene->lighting->directionalLight->direction.y, Math3d::Vec3f(0.0, 1.0, 0.0));

		//Current::player->currentCamera->viewMatrix.initCamera(pos + Current::player->position, Current::player->position, Math3d::Vec3f(0.0f, 1.0f, 0.0f));
		

		rot.initCamera(pos, Math3d::Vec3f(0.0f, 0.0f, 0.0f), Math3d::Vec3f(0.0f, 1.0f, 0.0f));
		//trans.rotate(Current::scene->lighting->directionalLight->direction);
		//trans.transform(pos);

		//testMat.rotate(-Current::scene->lighting->directionalLight->direction);
		//testMat.rotate(Current::scene->lighting->directionalLight->rotationVec);
		

		//testMat.initCamera(Math3d::Vec3f(0.0f, 0.0f, 0.0f), Current::scene->lighting->directionalLight->rotationVec, Math3d::Vec3f(0.0f, 1.0f, 0.0f));

		//testMat.rotate(Math3d::Vec3f(	Math3d::toDegree(Current::scene->lighting->directionalLight->rotationVec.x),
		//								Math3d::toDegree(Current::scene->lighting->directionalLight->rotationVec.y),
		//								Math3d::toDegree(Current::scene->lighting->directionalLight->rotationVec.z)	 ));
		
		testMat = rot;
		
		//testMat.rotate(Math3d::Vec3f(-Current::scene->lighting->directionalLight->direction.x, -Current::scene->lighting->directionalLight->direction.y, -Current::scene->lighting->directionalLight->direction.z));

		//testMat.rotate(Math3d::Vec3f(0.0f, 0.0f, -Current::scene->lighting->directionalLight->direction.z));
		//testMat.rotate(Math3d::Vec3f(-Current::scene->lighting->directionalLight->direction.x, 0.0f, 0.0f));
		//testMat.rotate(Math3d::Vec3f(0.0f, -Current::scene->lighting->directionalLight->direction.y, 0.0f));

		//testMat.transform(pos + Current::scene->player.currentCamera->position);

		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &testMat.m[0]);
	}
	test1PerFrameMatUniform::test1PerFrameMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "test1PerFrameMat";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void test1PerFrameMatUniform::updateUniform()
	{
		Math3d::Mat4f testMat;

		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &testMat.m[0]);
	}
	test1FloatPerModelUniform::test1FloatPerModelUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "test1PerModelFloat";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	void test1FloatPerModelUniform::updateUniform()
	{
		float testFloat = 0;

		glUniform1f(uniformID, testFloat);
	}
	
	void generateTextureUpdateList(std::vector<BaseUniformFunction*> *functionList_arg, GLuint *shaderProgram_arg)
	{
		positionTextureUniform positionTextureTemp(shaderProgram_arg);
		if(positionTextureTemp.testUniform())
			functionList_arg->push_back(new positionTextureUniform(shaderProgram_arg));
		diffuseTextureUniform diffuseTextureTemp(shaderProgram_arg);
		if(diffuseTextureTemp.testUniform())
			functionList_arg->push_back(new diffuseTextureUniform(shaderProgram_arg));
		normalTextureUniform normalTextureTemp(shaderProgram_arg);
		if(normalTextureTemp.testUniform())
			functionList_arg->push_back(new normalTextureUniform(shaderProgram_arg));
		
		sunGlowTextureUniform sunGlowTextureTemp(shaderProgram_arg);
		if(sunGlowTextureTemp.testUniform())
			functionList_arg->push_back(new sunGlowTextureUniform(shaderProgram_arg));
		skyMapTextureUniform skyMapTextureTemp(shaderProgram_arg);
		if(skyMapTextureTemp.testUniform())
			functionList_arg->push_back(new skyMapTextureUniform(shaderProgram_arg));
		
		dirShadowMapTextureUniform dirShadowMapTextureTemp(shaderProgram_arg);
		if(dirShadowMapTextureTemp.testUniform())
			functionList_arg->push_back(new dirShadowMapTextureUniform(shaderProgram_arg));
	}
	void generatePerFrameList(std::vector<BaseUniformFunction*> *functionList_arg, GLuint *shaderProgram_arg)
	{
		viewMatUniform viewMatTemp(shaderProgram_arg);
		if(viewMatTemp.testUniform())
			functionList_arg->push_back(new viewMatUniform(shaderProgram_arg));
		projectionMatUniform projectionMatTemp(shaderProgram_arg);
		if(projectionMatTemp.testUniform())
			functionList_arg->push_back(new projectionMatUniform(shaderProgram_arg));
		viewProjectionMatUniform viewProjectionMatTemp(shaderProgram_arg);
		if(viewProjectionMatTemp.testUniform())
			functionList_arg->push_back(new viewProjectionMatUniform(shaderProgram_arg));
		dirLightMatUniform dirLightMatTemp(shaderProgram_arg);
		if(dirLightMatTemp.testUniform())
			functionList_arg->push_back(new dirLightMatUniform(shaderProgram_arg));
		dirLightProjectionMatUniform dirLightProjectionMatTemp(shaderProgram_arg);
		if(dirLightProjectionMatTemp.testUniform())
			functionList_arg->push_back(new dirLightProjectionMatUniform(shaderProgram_arg));

		cameraPosUniform cameraPosTemp(shaderProgram_arg);
		if(cameraPosTemp.testUniform())
			functionList_arg->push_back(new cameraPosUniform(shaderProgram_arg));
		cameraUpUniform cameraUpTemp(shaderProgram_arg);
		if(cameraUpTemp.testUniform())
			functionList_arg->push_back(new cameraUpUniform(shaderProgram_arg));
		cameraRightUniform cameraRightTemp(shaderProgram_arg);
		if(cameraRightTemp.testUniform())
			functionList_arg->push_back(new cameraRightUniform(shaderProgram_arg));

		screenSizeUniform screenSizeTemp(shaderProgram_arg);
		if(screenSizeTemp.testUniform())
			functionList_arg->push_back(new screenSizeUniform(shaderProgram_arg));
		
		fogDensityUniform fogDensityTemp(shaderProgram_arg);
		if(fogDensityTemp.testUniform())
			functionList_arg->push_back(new fogDensityUniform(shaderProgram_arg));
		fogColorUniform fogColorTemp(shaderProgram_arg);
		if(fogColorTemp.testUniform())
			functionList_arg->push_back(new fogColorUniform(shaderProgram_arg));
		
		test1PerFrameMatUniform test1PerFrameMatTemp(shaderProgram_arg);
		if(test1PerFrameMatTemp.testUniform())
			functionList_arg->push_back(new test1PerFrameMatUniform(shaderProgram_arg));
	}
	void generatePerModelList(std::vector<BaseUniformFunction*> *functionList_arg, GLuint *shaderProgram_arg)
	{
		modelMatUniform modelMatTemp(shaderProgram_arg);
		if(modelMatTemp.testUniform())
			functionList_arg->push_back(new modelMatUniform(shaderProgram_arg));
		modelViewMatUniform modelViewMatTemp(shaderProgram_arg);
		if(modelViewMatTemp.testUniform())
			functionList_arg->push_back(new modelViewMatUniform(shaderProgram_arg));
		modelViewProjectionMatUniform modelViewProjectionMatTemp(shaderProgram_arg);
		if(modelViewProjectionMatTemp.testUniform())
			functionList_arg->push_back(new modelViewProjectionMatUniform(shaderProgram_arg));
		pointLightViewProjectionMatUniform pointLightViewProjectionMatTemp(shaderProgram_arg);
		if(pointLightViewProjectionMatTemp.testUniform())
			functionList_arg->push_back(new pointLightViewProjectionMatUniform(shaderProgram_arg));
		spotLightViewProjectionMatUniform spotLightViewProjectionMatTemp(shaderProgram_arg);
		if(spotLightViewProjectionMatTemp.testUniform())
			functionList_arg->push_back(new spotLightViewProjectionMatUniform(shaderProgram_arg));
		stencilPassViewProjectionMatUniform stencilPassViewProjectionMatTemp(shaderProgram_arg);
		if(stencilPassViewProjectionMatTemp.testUniform())
			functionList_arg->push_back(new stencilPassViewProjectionMatUniform(shaderProgram_arg));
		
		directionalLightBaseColorUniform directionalLightBaseColorTemp(shaderProgram_arg);
		if(directionalLightBaseColorTemp.testUniform())
			functionList_arg->push_back(new directionalLightBaseColorUniform(shaderProgram_arg));
		directionalLightBaseAmbientIntensityUniform directionalLightBaseAmbientIntensityTemp(shaderProgram_arg);
		if(directionalLightBaseAmbientIntensityTemp.testUniform())
			functionList_arg->push_back(new directionalLightBaseAmbientIntensityUniform(shaderProgram_arg));
		directionalLightDirectionUniform directionalLightDirectionTemp(shaderProgram_arg);
		if(directionalLightDirectionTemp.testUniform())
			functionList_arg->push_back(new directionalLightDirectionUniform(shaderProgram_arg));
		directionalLightDiffuseIntensityUniform directionalLightDiffuseIntensityTemp(shaderProgram_arg);
		if(directionalLightDiffuseIntensityTemp.testUniform())
			functionList_arg->push_back(new directionalLightDiffuseIntensityUniform(shaderProgram_arg));

		pointLightBaseColorUniform pointLightBaseColorTemp(shaderProgram_arg);
		if(pointLightBaseColorTemp.testUniform())
			functionList_arg->push_back(new pointLightBaseColorUniform(shaderProgram_arg));
		pointLightAmbientIntensityUniform pointLightAmbientIntensityTemp(shaderProgram_arg);
		if(pointLightAmbientIntensityTemp.testUniform())
			functionList_arg->push_back(new pointLightAmbientIntensityUniform(shaderProgram_arg));
		pointLightPositionUniform pointLightPositionTemp(shaderProgram_arg);
		if(pointLightPositionTemp.testUniform())
			functionList_arg->push_back(new pointLightPositionUniform(shaderProgram_arg));
		pointLightBaseDiffuseIntensityUniform pointLightBaseDiffuseIntensityTemp(shaderProgram_arg);
		if(pointLightBaseDiffuseIntensityTemp.testUniform())
			functionList_arg->push_back(new pointLightBaseDiffuseIntensityUniform(shaderProgram_arg));
		pointLightAttenuationConstantUniform pointLightAttenuationConstantTemp(shaderProgram_arg);
		if(pointLightAttenuationConstantTemp.testUniform())
			functionList_arg->push_back(new pointLightAttenuationConstantUniform(shaderProgram_arg));
		pointLightAttenuationLinearUniform pointLightAttenuationLinearTemp(shaderProgram_arg);
		if(pointLightAttenuationLinearTemp.testUniform())
			functionList_arg->push_back(new pointLightAttenuationLinearUniform(shaderProgram_arg));
		pointLightAttenuationExponentialUniform pointLightAttenuationExponentialTemp(shaderProgram_arg);
		if(pointLightAttenuationExponentialTemp.testUniform())
			functionList_arg->push_back(new pointLightAttenuationExponentialUniform(shaderProgram_arg));

		spotLightBaseColorUniform spotLightBaseColorTemp(shaderProgram_arg);
		if(spotLightBaseColorTemp.testUniform())
			functionList_arg->push_back(new spotLightBaseColorUniform(shaderProgram_arg));
		spotLightPositionUniform spotLightPositionTemp(shaderProgram_arg);
		if(spotLightPositionTemp.testUniform())
			functionList_arg->push_back(new spotLightPositionUniform(shaderProgram_arg));
		spotLightAmbientIntensityUniform spotLightAmbientIntensityTemp(shaderProgram_arg);
		if(spotLightAmbientIntensityTemp.testUniform())
			functionList_arg->push_back(new spotLightAmbientIntensityUniform(shaderProgram_arg));
		spotLightBaseDiffuseIntensityUniform spotLightBaseDiffuseIntensityTemp(shaderProgram_arg);
		if(spotLightBaseDiffuseIntensityTemp.testUniform())
			functionList_arg->push_back(new spotLightBaseDiffuseIntensityUniform(shaderProgram_arg));
		spotLightAttenuationConstantUniform spotLightAttenuationConstantTemp(shaderProgram_arg);
		if(spotLightAttenuationConstantTemp.testUniform())
			functionList_arg->push_back(new spotLightAttenuationConstantUniform(shaderProgram_arg));
		spotLightAttenuationLinearUniform spotLightAttenuationLinearTemp(shaderProgram_arg);
		if(spotLightAttenuationLinearTemp.testUniform())
			functionList_arg->push_back(new spotLightAttenuationLinearUniform(shaderProgram_arg));
		spotLightAttenuationExponentialUniform spotLightAttenuationExponentialTemp(shaderProgram_arg);
		if(spotLightAttenuationExponentialTemp.testUniform())
			functionList_arg->push_back(new spotLightAttenuationExponentialUniform(shaderProgram_arg));
		spotLightDirectionUniform spotLightDirectionTemp(shaderProgram_arg);
		if(spotLightDirectionTemp.testUniform())
			functionList_arg->push_back(new spotLightDirectionUniform(shaderProgram_arg));
		spotLightCutoffUniform spotLightCutoffTemp(shaderProgram_arg);
		if(spotLightCutoffTemp.testUniform())
			functionList_arg->push_back(new spotLightCutoffUniform(shaderProgram_arg));

		dirShadowMapMVPUniform dirShadowMapMVPTemp(shaderProgram_arg);
		if(dirShadowMapMVPTemp.testUniform())
			functionList_arg->push_back(new dirShadowMapMVPUniform(shaderProgram_arg));
		dirShadowMapBiasMVPUniform dirShadowMapBiasMVPTemp(shaderProgram_arg);
		if(dirShadowMapBiasMVPTemp.testUniform())
			functionList_arg->push_back(new dirShadowMapBiasMVPUniform(shaderProgram_arg));
		
		test1PerModelMatUniform test1PerModelMatTemp(shaderProgram_arg);
		if(test1PerModelMatTemp.testUniform())
			functionList_arg->push_back(new test1PerModelMatUniform(shaderProgram_arg));
		test1FloatPerModelUniform test1FloatPerModelTemp(shaderProgram_arg);
		if(test1FloatPerModelTemp.testUniform())
			functionList_arg->push_back(new test1FloatPerModelUniform(shaderProgram_arg));
	}
	void generatePerMeshList(std::vector<BaseUniformFunction*> *functionList_arg, GLuint *shaderProgram_arg)
	{

	}
}