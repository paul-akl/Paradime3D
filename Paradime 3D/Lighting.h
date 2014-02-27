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
		Math3d::Mat4f lightModelMat;
	    float ambientIntensity;
	    float diffuseIntensity;

		bool	colorModified,
				ambientIntensityModified,
				diffuseIntensityModified;

		bool updateRequired, active, deleteQue;
	
	    BaseLight()
	    {
	        color = Math3d::Vec3f(0.0f, 0.0f, 0.0f);
	        ambientIntensity = 0.0f;
	        diffuseIntensity = 0.0f;

			updateRequired  = true;
			active			= true;
			deleteQue		= false;
			
			colorModified			 = true;
			ambientIntensityModified = true;
			diffuseIntensityModified = true;
	    }
		virtual void updateLightModelMat() = 0;
	};
	struct DirectionalLight : public BaseLight
	{        
	    Math3d::Vec3f direction, rotationVec;
		Math3d::Mat4f MVP, shadowMapBias;
		bool directionUpdated, MVPUpdated;
	
	    DirectionalLight()
	    {
			MVPUpdated = true;
			directionUpdated = true;
	        direction = Math3d::Vec3f(0.0f, 0.0f, 0.0f);
			shadowMapBias = Math3d::Mat4f(	0.5f, 0.0f, 0.0f, 0.0f,
											0.0f, 0.5f, 0.0f, 0.0f,
											0.0f, 0.0f, 0.5f, 0.0f,
											0.5f, 0.5f, 0.5f, 1.0f	);
	    }
		DirectionalLight(Math3d::Vec3f color_arg, float ambientIntensity_arg, float diffuseIntensity_arg, Math3d::Vec3f direction_arg)
		{
			MVPUpdated		 = true;
			directionUpdated	 = true;
			color			 = color_arg;
			direction		 = direction_arg;
			ambientIntensity = ambientIntensity_arg;
			diffuseIntensity = diffuseIntensity_arg;
		}

		void calcShadowMapMat()
		{
			Math3d::Mat4f modelMat, viewMat, projectionMat;
			//modelMat = Math3d::Mat4f(1.0f);
			//viewMat.initCamera(Math3d::Vec3f(0.0f, 0.0f, 0.0f), -direction, Math3d::Vec3f(0.0f, 1.0f, 0.0f));
			//viewMat.initCamera(direction, Math3d::Vec3f(0.0f, 0.0f, 0.0f), Math3d::Vec3f(0.0f, 1.0f, 0.0f));
			//projectionMat.ortho(-10, 10, -10, 10, -10, 20);
			//projectionMat.ortho(-100, 100, -100, 100, 0, 400);

			Math3d::Vec3f position = Math3d::Vec3f(-50.0f, -50.0f, -50.0f);
			position.rotate(direction.z, Math3d::Vec3f(0.0f, 0.0f, 1.0f));
			position.rotate(direction.x, Math3d::Vec3f(1.0f, 0.0f, 0.0f));
			position.rotate(direction.y, Math3d::Vec3f(0.0f, 1.0f, 0.0f));

			modelMat.rotate(direction);

			projectionMat.identity();
			projectionMat.ortho(-50, 50, -50, 50, 100, 200);
			viewMat.initCamera(position, Math3d::Vec3f(0.0f, 0.0f, 0.0f), Math3d::Vec3f(0.0f, 1.0f, 0.0f));

			//modelMat.transform(position);
			//modelMat.rotate(direction);
			//modelMat.rotate(Math3d::Vec3f(90.0f, 0.0f, 0.0f));
			//modelMat.transform(Math3d::Vec3f(0.0f, 5.0f, 0.0f));
		
			MVP = projectionMat * viewMat;// * modelMat;
			//MVP = projectionMat * modelMat;
			//MVPUpdated = false;
		}
		void calcRotationVec()
		{
			rotationVec = Math3d::Vec3f(1.0, 1.0, 1.0);
			
			rotationVec.rotate(direction.z, Math3d::Vec3f(0.0, 0.0, 1.0));
			rotationVec.rotate(direction.x, Math3d::Vec3f(1.0, 0.0, 0.0));
			rotationVec.rotate(direction.y, Math3d::Vec3f(0.0, 1.0, 0.0));
			
			//directionUpdated = false;
		}
		void updateLightModelMat()
		{

		}
	};
	struct PointLight : public BaseLight
	{
	    Math3d::Vec3f position;
		float modelScale;
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
			modelScale = 0.0f;
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
			
			modelScale = 0.0f;
			modified	= true;
		}
		virtual void calculateModelScale()
		{
			float maxChannel = (Math3d::getMax(Math3d::getMax(color.x, color.y), color.z) * diffuseIntensity);
			//sphereScale = (32.0f * sqrtf(maxChannel) + 1.0f);
			modelScale = (3.0f * sqrtf(maxChannel) / attenuation.exp);

			modified = false;
		}
		void updateLightModelMat()
		{
			calculateModelScale();
			lightModelMat.identity();
			lightModelMat.transform(position);
			lightModelMat.scale(modelScale);
		}
	};
	struct SpotLight : public PointLight
	{
	    Math3d::Vec3f direction, rotationVec;
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
		virtual void calculateModelScale()
		{
			modelScale = 200.0f;

			modified = false;
		}
		void calcRotationVec()
		{
			rotationVec = Math3d::Vec3f(1.0, 1.0, 1.0);
			
			rotationVec.rotate(direction.z, Math3d::Vec3f(0.0, 0.0, 1.0));
			rotationVec.rotate(direction.x, Math3d::Vec3f(1.0, 0.0, 0.0));
			rotationVec.rotate(direction.y, Math3d::Vec3f(0.0, 1.0, 0.0));
			
			//directionUpdated = false;
		}
		void updateLightModelMat()
		{
			calculateModelScale();
			lightModelMat.identity();
			lightModelMat.transform(position);
			lightModelMat.scale(modelScale);
		}
	};
	
	int pointLightPoolSize, spotLightPoolSize;

	DirectionalLight *directionalLight;
	PointLight		 *currentPointLight;
	SpotLight		 *currentSpotLight;
	BaseLight	*currentLight;
	
	std::vector<PointLight*> pointLightPool;
	std::vector<SpotLight*>	spotLightPool;

	Lighting();
	~Lighting();

	void init();

	void addDirectionalLight(Math3d::Vec3f color_arg, float ambientIntensity_arg, float diffuseIntensity_arg, Math3d::Vec3f direction_arg);
	void addPointLight(Math3d::Vec3f color_arg, float ambientIntensity_arg, float diffuseIntensity_arg, Math3d::Vec3f position_arg, float constant_arg, float linear_arg, float exp_arg);
	void addSpotLight(Math3d::Vec3f color_arg, float ambientIntensity_arg, float diffuseIntensity_arg, Math3d::Vec3f position_arg, float constant_arg, float linear_arg, float exp_arg, Math3d::Vec3f direction_arg, float cutoff_arg);
};

#endif