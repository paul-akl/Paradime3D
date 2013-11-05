#include "ErrorHandler.h"
#include "Lighting.h"

Lighting::Lighting()
{
	Message::show(MSG_INFO, MSG_LIGHT, "Lighting handle has been created.");
}
Lighting::~Lighting()
{
	// Delete all the lights and models to clear memory
	delete directionalLight;

	for(int i=0; i < pointLightPoolSize; i++)
		delete pointLightPool[i];

	for(int i=0; i < spotLightPoolSize; i++)
		delete spotLightPool[i];

	delete dirLightQuad;
	delete pointLightSphere;
}

void Lighting::init()
{
	directionalLight	= NULL;
	pointLightPoolSize	= 0;
	spotLightPoolSize	= 0;

	dirLightQuad = new ModelLoader::GenericModel("quad.obj");
	pointLightSphere = new ModelLoader::GenericModel("sphere.obj");
}
void Lighting::initShaders(	std::string dirLightVertexShader_arg, std::string dirLightFragmentShader_arg, 
							std::string pointLightVertexShader_arg, std::string pointLightFragmentShader_arg, 
							std::string spotLightVertexShader_arg, std::string spotLightFragmentShader_arg		)
{
	try
	{
		dirLightShader	 = ShaderLoader::load(dirLightVertexShader_arg, dirLightFragmentShader_arg);
		pointLightShader = ShaderLoader::load(pointLightVertexShader_arg, pointLightFragmentShader_arg);
		spotLightShader	 = ShaderLoader::load(spotLightVertexShader_arg, spotLightFragmentShader_arg);
	}
	catch(Message::messageCode error_arg)
	{
		error_arg.display();
	}
	
	Message::show(MSG_INFO, MSG_LIGHT, "Lighting shaders has been initialized.");
}

void Lighting::renderDirectionLight()
{
	dirLightShader->bind();
}
void Lighting::renderPointLights()
{
	pointLightShader->bind();

	for(int i=0; i < pointLightPoolSize; i++)
	{
		currentPointLight = pointLightPool[i];
		//pointLightShader->
	}
}
void Lighting::renderSpotLights()
{
	spotLightShader->bind();
}

void Lighting::addDirectionalLight(Math3d::Vec3f color_arg, float ambientIntensity_arg, float diffuseIntensity_arg, Math3d::Vec3f direction_arg)
{
	if(directionalLight != NULL)
		delete directionalLight;

	directionalLight = new Lighting::DirectionalLight(color_arg, ambientIntensity_arg, diffuseIntensity_arg, direction_arg);

	std::cout << "DIRECTIONAL LIGHT CREATED" << std::endl;
}
void Lighting::addPointLight(Math3d::Vec3f color_arg, float ambientIntensity_arg, float diffuseIntensity_arg, Math3d::Vec3f position_arg, float constant_arg, float linear_arg, float exp_arg)
{
	pointLightPool.push_back(new Lighting::PointLight(color_arg, ambientIntensity_arg, diffuseIntensity_arg, position_arg, constant_arg, linear_arg, exp_arg));
	pointLightPoolSize = pointLightPool.size();

	//return pointLightPool[pointLightPoolSize - 1];
}
void Lighting::addSpotLight(Math3d::Vec3f color_arg, float ambientIntensity_arg, float diffuseIntensity_arg, Math3d::Vec3f position_arg, float constant_arg, float lienar_arg, float exp_arg, Math3d::Vec3f direction_arg, float cutoff_arg)
{
	spotLightPool.push_back(new Lighting::SpotLight(color_arg, ambientIntensity_arg, diffuseIntensity_arg, position_arg, constant_arg, lienar_arg, exp_arg, direction_arg, cutoff_arg));
	spotLightPoolSize = spotLightPool.size();

	//return spotLightPool[spotLightPoolSize - 1];
}