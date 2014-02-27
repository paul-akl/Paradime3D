#ifndef ADAPTIVE_UNIFORMS_FUNCTIONS_NAMESPACE
#define ADAPTIVE_UNIFORMS_FUNCTIONS_NAMESPACE

#include <GL\glew.h>
#include <string>
#include <vector>

namespace UniformFunctions
{
	class BaseUniformFunction
	{
	public:
		GLuint uniformID;
		std::string uniformName;
		virtual bool testUniform()
		{
			return (uniformID == -1) ? false : true;
		}
		virtual void updateUniform() = 0;
	};
	
	class modelMatUniform : public BaseUniformFunction
	{
	public:
		modelMatUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class viewMatUniform : public BaseUniformFunction
	{
	public:
		viewMatUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class projectionMatUniform : public BaseUniformFunction
	{
	public:
		projectionMatUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class viewProjectionMatUniform : public BaseUniformFunction
	{
	public:
		viewProjectionMatUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class modelViewMatUniform : public BaseUniformFunction
	{
	public:
		modelViewMatUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class modelViewProjectionMatUniform : public BaseUniformFunction
	{
	public:
		modelViewProjectionMatUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class screenSizeUniform : public BaseUniformFunction
	{
	public:
		screenSizeUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class numPointLightsUniform : public BaseUniformFunction
	{
	public:
		numPointLightsUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class numSpotLightsUniform : public BaseUniformFunction
	{
	public:
		numSpotLightsUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class pointLightViewProjectionMatUniform : public BaseUniformFunction
	{
	public:
		pointLightViewProjectionMatUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class spotLightViewProjectionMatUniform : public BaseUniformFunction
	{
	public:
		spotLightViewProjectionMatUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class stencilPassViewProjectionMatUniform : public BaseUniformFunction
	{
	public:
		stencilPassViewProjectionMatUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class dirLightMatUniform : public BaseUniformFunction
	{
	public:
		dirLightMatUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class dirLightProjectionMatUniform : public BaseUniformFunction
	{
	public:
		dirLightProjectionMatUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};

	class directionalLightBaseColorUniform : public BaseUniformFunction
	{
	public:
		directionalLightBaseColorUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class directionalLightBaseAmbientIntensityUniform : public BaseUniformFunction
	{
	public:
		directionalLightBaseAmbientIntensityUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class directionalLightDirectionUniform : public BaseUniformFunction
	{
	public:
		directionalLightDirectionUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class directionalLightDiffuseIntensityUniform : public BaseUniformFunction
	{
	public:
		directionalLightDiffuseIntensityUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};

	class pointLightBaseColorUniform : public BaseUniformFunction
	{
	public:
		pointLightBaseColorUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class pointLightAmbientIntensityUniform : public BaseUniformFunction
	{
	public:
		pointLightAmbientIntensityUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class pointLightPositionUniform : public BaseUniformFunction
	{
	public:
		pointLightPositionUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class pointLightBaseDiffuseIntensityUniform : public BaseUniformFunction
	{
	public:
		pointLightBaseDiffuseIntensityUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class pointLightAttenuationConstantUniform : public BaseUniformFunction
	{
	public:
		pointLightAttenuationConstantUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class pointLightAttenuationLinearUniform : public BaseUniformFunction
	{
	public:
		pointLightAttenuationLinearUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class pointLightAttenuationExponentialUniform : public BaseUniformFunction
	{
	public:
		pointLightAttenuationExponentialUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	
	class spotLightBaseColorUniform : public BaseUniformFunction
	{
	public:
		spotLightBaseColorUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class spotLightPositionUniform : public BaseUniformFunction
	{
	public:
		spotLightPositionUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class spotLightAmbientIntensityUniform : public BaseUniformFunction
	{
	public:
		spotLightAmbientIntensityUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class spotLightBaseDiffuseIntensityUniform : public BaseUniformFunction
	{
	public:
		spotLightBaseDiffuseIntensityUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class spotLightAttenuationConstantUniform : public BaseUniformFunction
	{
	public:
		spotLightAttenuationConstantUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class spotLightAttenuationLinearUniform : public BaseUniformFunction
	{
	public:
		spotLightAttenuationLinearUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class spotLightAttenuationExponentialUniform : public BaseUniformFunction
	{
	public:
		spotLightAttenuationExponentialUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class spotLightDirectionUniform : public BaseUniformFunction
	{
	public:
		spotLightDirectionUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class spotLightCutoffUniform : public BaseUniformFunction
	{
	public:
		spotLightCutoffUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	
	class dirShadowMapMVPUniform : public BaseUniformFunction
	{
	public:
		dirShadowMapMVPUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class dirShadowMapBiasMVPUniform : public BaseUniformFunction
	{
	public:
		dirShadowMapBiasMVPUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	
	class cameraPosUniform : public BaseUniformFunction
	{
	public:
		cameraPosUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class cameraUpUniform : public BaseUniformFunction
	{
	public:
		cameraUpUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class cameraRightUniform : public BaseUniformFunction
	{
	public:
		cameraRightUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};

	class positionTextureUniform : public BaseUniformFunction
	{
	public:
		positionTextureUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class diffuseTextureUniform : public BaseUniformFunction
	{
	public:
		diffuseTextureUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class normalTextureUniform : public BaseUniformFunction
	{
	public:
		normalTextureUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	
	class sunGlowTextureUniform : public BaseUniformFunction
	{
	public:
		sunGlowTextureUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class skyMapTextureUniform : public BaseUniformFunction
	{
	public:
		skyMapTextureUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class dirShadowMapTextureUniform : public BaseUniformFunction
	{
	public:
		dirShadowMapTextureUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	
	class fogDensityUniform : public BaseUniformFunction
	{
	public:
		fogDensityUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class fogColorUniform : public BaseUniformFunction
	{
	public:
		fogColorUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	
	class test1PerModelMatUniform : public BaseUniformFunction
	{
	public:
		test1PerModelMatUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class test1PerFrameMatUniform : public BaseUniformFunction
	{
	public:
		test1PerFrameMatUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};
	class test1FloatPerModelUniform : public BaseUniformFunction
	{
	public:
		test1FloatPerModelUniform(GLuint *shaderProgram_arg);
		void updateUniform();
	};

	void init();
	void generateTextureUpdateList(std::vector<BaseUniformFunction*> *functionList_arg, GLuint *shaderProgram_arg);
	void generatePerFrameList(std::vector<BaseUniformFunction*> *functionList_arg, GLuint *shaderProgram_arg);
	void generatePerModelList(std::vector<BaseUniformFunction*> *functionList_arg, GLuint *shaderProgram_arg);
	void generatePerMeshList(std::vector<BaseUniformFunction*> *functionList_arg, GLuint *shaderProgram_arg);
}

#endif