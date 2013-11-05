#ifndef LIGHTING_CLASS
#define LIGHTING_CLASS

#include <vector>

#include "Math.h"
#include "Loader_Shader.h"
#include "Loader_Model.h"

class Lighting
{
public:
	
	struct BaseLight
	{
	    Math3d::Vec3f color;
	    float ambientIntensity;
	    float diffuseIntensity;

		bool updateRequired, active, deleteQue;
	
	    BaseLight()
	    {
	        color = Math3d::Vec3f(0.0f, 0.0f, 0.0f);
	        ambientIntensity = 0.0f;
	        diffuseIntensity = 0.0f;

			updateRequired  = true;
			active			= true;
			deleteQue		= false;
	    }
	};
	struct DirectionalLight : public BaseLight
	{        
	    Math3d::Vec3f direction;
	
	    DirectionalLight()
	    {
	        direction = Math3d::Vec3f(0.0f, 0.0f, 0.0f);
	    }
		DirectionalLight(Math3d::Vec3f color_arg, float ambientIntensity_arg, float diffuseIntensity_arg, Math3d::Vec3f direction_arg)
		{
			color			 = color_arg;
			direction		 = direction_arg;
			ambientIntensity = ambientIntensity_arg;
			diffuseIntensity = diffuseIntensity_arg;
		}
	};
	struct PointLight : public BaseLight
	{
	    Math3d::Vec3f position;
		Math3d::Mat4f lightSphereMat;
		float sphereScale;
		bool modified;
	
	    struct
	    {
	        float constant;
	        float linear;
	        float exp;
	    } attenuation;
	
	    PointLight()
	    {
	        position = Math3d::Vec3f(0.0f, 0.0f, 0.0f);
	        attenuation.constant = 1.0f;
	        attenuation.linear = 0.0f;
	        attenuation.exp = 0.0f;
			sphereScale = 0.0f;
			modified = true;
	    }
		PointLight(Math3d::Vec3f color_arg, float ambientIntensity_arg, float diffuseIntensity_arg, Math3d::Vec3f position_arg, float constant_arg, float linear_arg, float exp_arg)
		{
			color			 = color_arg;
			position		 = position_arg;
			ambientIntensity = ambientIntensity_arg;
			diffuseIntensity = diffuseIntensity_arg;
			
	        attenuation.constant = constant_arg;
	        attenuation.linear	 = linear_arg;
	        attenuation.exp		 = exp_arg;
			
			sphereScale = 0.0f;
			modified	= true;
		}
		void calculateSphere()
		{
			float maxChannel = (Math3d::getMax(Math3d::getMax(color.x, color.y), color.z) * diffuseIntensity);
			sphereScale = (32.0f * sqrtf(maxChannel) + 1.0f);
			modified = false;
		}
	};
	struct SpotLight : public PointLight
	{
	    Math3d::Vec3f direction;
	    float cutoff;
	
	    SpotLight()
	    {
	        direction = Math3d::Vec3f(0.0f, 0.0f, 0.0f);
	        cutoff = 0.0f;
	    }
		SpotLight(Math3d::Vec3f color_arg, float ambientIntensity_arg, float diffuseIntensity_arg, Math3d::Vec3f position_arg, float constant_arg, float lienar_arg, float exp_arg, Math3d::Vec3f direction_arg, float cutoff_arg)
		{
			color			 = color_arg;
			cutoff			 = cutoff_arg;
			position		 = position_arg;
			direction		 = direction_arg;
			ambientIntensity = ambientIntensity_arg;
			diffuseIntensity = diffuseIntensity_arg;
			
	        attenuation.constant = constant_arg;
	        attenuation.linear	 = lienar_arg;
	        attenuation.exp		 = exp_arg;
		}
	};
	
	int pointLightPoolSize, spotLightPoolSize;

	ModelLoader::GenericModel *dirLightQuad, *pointLightSphere;

	DirectionalLight *directionalLight;
	PointLight		 *currentPointLight;
	SpotLight		 *currentSpotLight;
	
	std::vector<PointLight*> pointLightPool;
	std::vector<SpotLight*>	spotLightPool;
	
	ShaderLoader::Shader *dirLightShader, *pointLightShader, *spotLightShader;

	Lighting();
	~Lighting();

	void init();
	void initShaders(	std::string dirLightVertexShader_arg,	std::string dirLightFragmentShader_arg, 
						std::string pointLightVertexShader_arg, std::string pointLightFragmentShader_arg, 
						std::string spotLightVertexShader_arg,	std::string spotLightFragmentShader_arg		);

	//void initDirLightShader();
	//void initPointLightShader();
	//void initSpotLightShader();

	void renderDirectionLight();
	void renderPointLights();
	void renderSpotLights();

	void addDirectionalLight(Math3d::Vec3f color_arg, float ambientIntensity_arg, float diffuseIntensity_arg, Math3d::Vec3f direction_arg);
	void addPointLight(Math3d::Vec3f color_arg, float ambientIntensity_arg, float diffuseIntensity_arg, Math3d::Vec3f position_arg, float constant_arg, float lienar_arg, float exp_arg);
	void addSpotLight(Math3d::Vec3f color_arg, float ambientIntensity_arg, float diffuseIntensity_arg, Math3d::Vec3f position_arg, float constant_arg, float lienar_arg, float exp_arg, Math3d::Vec3f direction_arg, float cutoff_arg);

//private:

};

#endif