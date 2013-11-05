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
	bool modelMatUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
	}
	void modelMatUniform::updateUniform()
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
			
			glUniformMatrix4fv(uniformID, 1, GL_FALSE, &Current::perspective->perspectiveMat.m[0]);
		}

		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &Current::objectParameters->modelMat.m[0]);
		
		//Math3d::Mat4f lightViewProj = Current::perspective->perspectiveMat * Current::player->currentCamera->viewMatrix;

		//glUniformMatrix4fv(uniformID, 1, GL_FALSE, &lightViewProj.m[0]);

	}
	
	viewMatUniform::viewMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "View";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	bool viewMatUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
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
	bool projectionMatUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
	}
	void projectionMatUniform::updateUniform()
	{
		if(Current::perspective->modified)
		{
			Current::perspective->modified = false;
			Current::perspective->perspectiveMat.perspective(Config::graphics::FOV, 
															 Config::currentScreenWidth, Config::currentScreenHeight, 
															 Config::engine::z_near, Config::engine::z_far);
			
			glUniformMatrix4fv(uniformID, 1, GL_FALSE, &Current::perspective->perspectiveMat.m[0]);
		}
	}
	
	viewProjectionMatUniform::viewProjectionMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "VP";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	bool viewProjectionMatUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
	}
	void viewProjectionMatUniform::updateUniform()
	{
		if(Current::perspective->modified)
		{
			Current::perspective->modified == false;
			Current::perspective->perspectiveMat.perspective(Config::graphics::FOV, 
															 Config::currentScreenWidth, Config::currentScreenHeight, 
															 Config::engine::z_near, Config::engine::z_far);
		}

		Math3d::Mat4f viewProjMat = Current::perspective->perspectiveMat * Current::player->currentCamera->viewMatrix;

		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &viewProjMat.m[0]);
	}

	modelViewMatUniform::modelViewMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "MV";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	bool modelViewMatUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
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
	bool modelViewProjectionMatUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
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
		
		Current::objectParameters->modelViewProjMat = Current::perspective->perspectiveMat * Current::player->currentCamera->viewMatrix * Current::objectParameters->modelMat;
		
		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &Current::objectParameters->modelViewProjMat.m[0]);
	}
	
	cameraPosUniform::cameraPosUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "cameraPos";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	bool cameraPosUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
	}
	void cameraPosUniform::updateUniform()
	{
		glUniform3f(uniformID, Current::player->currentCamera->position.x, Current::player->currentCamera->position.y, Current::player->currentCamera->position.z);
	}
	
	screenSizeUniform::screenSizeUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "screenSize";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	bool screenSizeUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
	}
	void screenSizeUniform::updateUniform()
	{
		glUniform2f(uniformID, Config::currentScreenWidth, Config::currentScreenHeight);
	}

	numPointLightsUniform::numPointLightsUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "numPointLights";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	bool numPointLightsUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
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
	bool numSpotLightsUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
	}
	void numSpotLightsUniform::updateUniform()
	{

	}
	
	pointLightViewProjectionMatUniform::pointLightViewProjectionMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "pointLightMVP";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	bool pointLightViewProjectionMatUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
	}
	void pointLightViewProjectionMatUniform::updateUniform()
	{
		//if(Current::scene->lighting->currentPointLight->modified)
		//{
			Current::scene->lighting->currentPointLight->calculateSphere();

			Current::scene->lighting->currentPointLight->lightSphereMat.identity();
			Current::scene->lighting->currentPointLight->lightSphereMat.transform(Current::scene->lighting->currentPointLight->position);
			Current::scene->lighting->currentPointLight->lightSphereMat.scale(Current::scene->lighting->currentPointLight->sphereScale);
			//Current::scene->lighting->currentPointLight->lightSphereMat.scale(1);
		//}

		//if(Current::perspective->modified)
		//{
		//	Current::perspective->modified == false;
			Current::perspective->perspectiveMat.perspective(Config::graphics::FOV, 
															 Config::currentScreenWidth, Config::currentScreenHeight, 
															 Config::engine::z_near, Config::engine::z_far);
		//}
		Math3d::Mat4f lightViewProj = Current::perspective->perspectiveMat * Current::player->currentCamera->viewMatrix * Current::scene->lighting->currentPointLight->lightSphereMat;

		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &lightViewProj.m[0]);
	}
	
	dirLightMatUniform::dirLightMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "dirLightMat";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	bool dirLightMatUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
	}
	void dirLightMatUniform::updateUniform()
	{
		Math3d::Mat4f lightMat;

		lightMat.identity();
		lightMat.rotate(Math3d::Vec3f(Config::renderer::dir_light_quad_rotation_x, Config::renderer::dir_light_quad_rotation_y, Config::renderer::dir_light_quad_rotation_z));
		//lightMat.transform(Math3d::Vec3f(Config::renderer::dir_light_quad_offset_x, Config::renderer::dir_light_quad_offset_y, Config::renderer::dir_light_quad_offset_z));

		Math3d::Mat4f viewProjMat = lightMat;

		glUniformMatrix4fv(uniformID, 1, GL_FALSE, &viewProjMat.m[0]);
	}

	dirLightProjectionMatUniform::dirLightProjectionMatUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "dirLightP";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	bool dirLightProjectionMatUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
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
	bool directionalLightBaseColorUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
	}
	void directionalLightBaseColorUniform::updateUniform()
	{
		glUniform3f(uniformID, Current::scene->lighting->directionalLight->color.x, Current::scene->lighting->directionalLight->color.y, Current::scene->lighting->directionalLight->color.z);
	}

	directionalLightBaseAmbientIntensityUniform::directionalLightBaseAmbientIntensityUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "directionalLight.base.ambientIntensity";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	bool directionalLightBaseAmbientIntensityUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
	}
	void directionalLightBaseAmbientIntensityUniform::updateUniform()
	{
		glUniform1f(uniformID, Current::scene->lighting->directionalLight->ambientIntensity);
	}

	directionalLightDirectionUniform::directionalLightDirectionUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "directionalLight.direction";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	bool directionalLightDirectionUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
	}
	void directionalLightDirectionUniform::updateUniform()
	{
		glUniform3f(uniformID, Current::scene->lighting->directionalLight->direction.x, Current::scene->lighting->directionalLight->direction.y, Current::scene->lighting->directionalLight->direction.z);
	}

	directionalLightDiffuseIntensityUniform::directionalLightDiffuseIntensityUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "directionalLight.base.diffuseIntensity";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	bool directionalLightDiffuseIntensityUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
	}
	void directionalLightDiffuseIntensityUniform::updateUniform()
	{
		glUniform1f(uniformID, Current::scene->lighting->directionalLight->diffuseIntensity);
	}
	
	pointLightBaseColorUniform::pointLightBaseColorUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "pointLight.base.color";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	bool pointLightBaseColorUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
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
	bool pointLightAmbientIntensityUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
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
	bool pointLightPositionUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
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
	bool pointLightBaseDiffuseIntensityUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
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
	bool pointLightAttenuationConstantUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
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
	bool pointLightAttenuationLinearUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
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
	bool pointLightAttenuationExponentialUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
	}
	void pointLightAttenuationExponentialUniform::updateUniform()
	{
		glUniform1f(uniformID, Current::scene->lighting->currentPointLight->attenuation.exp);
	}

	spotLightsUniform::spotLightsUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "spotLights";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	bool spotLightsUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
	}
	void spotLightsUniform::updateUniform()
	{

	}

	specularIntensityUniform::specularIntensityUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "specularIntensity";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	bool specularIntensityUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
	}
	void specularIntensityUniform::updateUniform()
	{
		glUniform1f(uniformID, Config::texture::default_specular_intensity);
	}
	
	specularPowerUniform::specularPowerUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "specularPower";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	bool specularPowerUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
	}
	void specularPowerUniform::updateUniform()
	{
		glUniform1f(uniformID, Config::texture::default_specular_power);
	}
	
	positionTextureUniform::positionTextureUniform(GLuint *shaderProgram_arg)
	{
		uniformName = "positionMap";
		uniformID = glGetUniformLocation(*shaderProgram_arg, uniformName.c_str());
	}
	bool positionTextureUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
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
	bool diffuseTextureUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
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
	bool normalTextureUniform::testUniform()
	{
		return (uniformID == -1) ? false : true;
	}
	void normalTextureUniform::updateUniform()
	{
		glUniform1i(uniformID, Current::normalMap);
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
		screenSizeUniform screenSizeTemp(shaderProgram_arg);
		if(screenSizeTemp.testUniform())
			functionList_arg->push_back(new screenSizeUniform(shaderProgram_arg));
		
		specularIntensityUniform specularIntensityTemp(shaderProgram_arg);
		if(specularIntensityTemp.testUniform())
			functionList_arg->push_back(new specularIntensityUniform(shaderProgram_arg));
		specularPowerUniform specularPowerTemp(shaderProgram_arg);
		if(specularPowerTemp.testUniform())
			functionList_arg->push_back(new specularPowerUniform(shaderProgram_arg));
		
		//numPointLightsUniform numPointLightsTemp(shaderProgram_arg);
		//if(numPointLightsTemp.testUniform())
		//	functionList_arg->push_back(new numPointLightsUniform(shaderProgram_arg));
		
		//numSpotLightsUniform numSpotLightsTemp(shaderProgram_arg);
		//if(numSpotLightsTemp.testUniform())
		//	functionList_arg->push_back(new numSpotLightsUniform(shaderProgram_arg));
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

		spotLightsUniform spotLightsTemp(shaderProgram_arg);
		if(spotLightsTemp.testUniform())
			functionList_arg->push_back(new spotLightsUniform(shaderProgram_arg));
	}
	void generatePerMeshList(std::vector<BaseUniformFunction*> *functionList_arg, GLuint *shaderProgram_arg)
	{

	}
}