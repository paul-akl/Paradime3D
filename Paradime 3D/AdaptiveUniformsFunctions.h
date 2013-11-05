#ifndef ADAPTIVE_UNIFORMS_FUNCTIONS_NAMESPACE
#define ADAPTIVE_UNIFORMS_FUNCTIONS_NAMESPACE

#include <GL\glew.h>
#include <string>
#include <vector>

//#include "CurrentUpdateState.h"

namespace UniformFunctions
{
	class BaseUniformFunction
	{
	public:
		GLuint uniformID;
		std::string uniformName;
		virtual bool testUniform() = 0;
		virtual void updateUniform() = 0;
	};
	
	class modelMatUniform : public BaseUniformFunction
	{
	public:
		modelMatUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class viewMatUniform : public BaseUniformFunction
	{
	public:
		viewMatUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class projectionMatUniform : public BaseUniformFunction
	{
	public:
		projectionMatUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class viewProjectionMatUniform : public BaseUniformFunction
	{
	public:
		viewProjectionMatUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class modelViewMatUniform : public BaseUniformFunction
	{
	public:
		modelViewMatUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class modelViewProjectionMatUniform : public BaseUniformFunction
	{
	public:
		modelViewProjectionMatUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class cameraPosUniform : public BaseUniformFunction
	{
	public:
		cameraPosUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class screenSizeUniform : public BaseUniformFunction
	{
	public:
		screenSizeUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class numPointLightsUniform : public BaseUniformFunction
	{
	public:
		numPointLightsUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class numSpotLightsUniform : public BaseUniformFunction
	{
	public:
		numSpotLightsUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class pointLightViewProjectionMatUniform : public BaseUniformFunction
	{
	public:
		pointLightViewProjectionMatUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class dirLightMatUniform : public BaseUniformFunction
	{
	public:
		dirLightMatUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class dirLightProjectionMatUniform : public BaseUniformFunction
	{
	public:
		dirLightProjectionMatUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};

	class directionalLightBaseColorUniform : public BaseUniformFunction
	{
	public:
		directionalLightBaseColorUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class directionalLightBaseAmbientIntensityUniform : public BaseUniformFunction
	{
	public:
		directionalLightBaseAmbientIntensityUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class directionalLightDirectionUniform : public BaseUniformFunction
	{
	public:
		directionalLightDirectionUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class directionalLightDiffuseIntensityUniform : public BaseUniformFunction
	{
	public:
		directionalLightDiffuseIntensityUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};

	class pointLightBaseColorUniform : public BaseUniformFunction
	{
	public:
		pointLightBaseColorUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class pointLightAmbientIntensityUniform : public BaseUniformFunction
	{
	public:
		pointLightAmbientIntensityUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class pointLightPositionUniform : public BaseUniformFunction
	{
	public:
		pointLightPositionUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class pointLightBaseDiffuseIntensityUniform : public BaseUniformFunction
	{
	public:
		pointLightBaseDiffuseIntensityUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class pointLightAttenuationConstantUniform : public BaseUniformFunction
	{
	public:
		pointLightAttenuationConstantUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class pointLightAttenuationLinearUniform : public BaseUniformFunction
	{
	public:
		pointLightAttenuationLinearUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class pointLightAttenuationExponentialUniform : public BaseUniformFunction
	{
	public:
		pointLightAttenuationExponentialUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};

	class spotLightsUniform : public BaseUniformFunction
	{
	public:
		spotLightsUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class specularIntensityUniform : public BaseUniformFunction
	{
	public:
		specularIntensityUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class specularPowerUniform : public BaseUniformFunction
	{
	public:
		specularPowerUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};

	class positionTextureUniform : public BaseUniformFunction
	{
	public:
		positionTextureUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class diffuseTextureUniform : public BaseUniformFunction
	{
	public:
		diffuseTextureUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};
	class normalTextureUniform : public BaseUniformFunction
	{
	public:
		normalTextureUniform(GLuint *shaderProgram_arg);
		bool testUniform();
		void updateUniform();
	};

	void init();
	void generateTextureUpdateList(std::vector<BaseUniformFunction*> *functionList_arg, GLuint *shaderProgram_arg);
	void generatePerFrameList(std::vector<BaseUniformFunction*> *functionList_arg, GLuint *shaderProgram_arg);
	void generatePerModelList(std::vector<BaseUniformFunction*> *functionList_arg, GLuint *shaderProgram_arg);
	void generatePerMeshList(std::vector<BaseUniformFunction*> *functionList_arg, GLuint *shaderProgram_arg);
}

#endif