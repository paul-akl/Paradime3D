#ifndef GAME_OBJECT_NAMESPACE
#define GAME_OBJECT_NAMESPACE

#include <vector>

#include "CommonClasses.h"
#include "Config.h"
#include "ErrorHandler.h"
#include "Loader_Config.h"
#include "Loader_Model.h"
#include "Loader_Shader.h"
#include "Loader_Texture.h"
#include "Math.h"

class GameObjects
{
public:

	GameObjects();
	~GameObjects();

	void init();
	void uninit();
	
	void load(std::string fileName_arg, Math3d::Vec3f position_arg, Math3d::Vec3f rotation_arg, Math3d::Vec3f scale_arg);
	void unload(std::string fileName_arg);

	void setPlayer(Math3d::Vec3f *position_arg);
	void setPlayer(Math3d::Vec3f *position_arg, Math3d::Vec3f *target_arg);

	void render();
	void render(ShaderLoader::BaseShader *shader_arg);
	void update();
	
private:
	
	class Object
	{
	public:
		
		class ObjectLoaderParameters
		{
		public:

			ObjectLoaderParameters();
			~ObjectLoaderParameters();

			struct MaterialNameAndIndex
			{
				MaterialNameAndIndex(std::string textureName_arg, unsigned int index_arg)
				{
					materialName = textureName_arg;
					index = index_arg;
				}

				std::string materialName;
				unsigned int index;
			};

			std::string modelName,
						vertexShaderName,
						fragmentShaderName;

			std::vector<MaterialNameAndIndex*> materialsFromFile;
		};

		Object(std::string fileName_arg);
		~Object();

		void render();
		void render(ShaderLoader::BaseShader *shader_arg);
		void update();
		bool compareFileName(std::string fileName_arg);

		std::string								objectFileName;
		ModelLoader::Model						*model;
		ShaderLoader::VertFragShader			*shader;
		std::vector<TextureLoader::Texture2D*>	texturePool;
		ObjectLoaderParameters					objectLoaderParam;
		Common::ObjectParameters				*objectParameters;
		
	private:

		GLuint positionUniform;
		ConfigFile gameObjectFile;

		void loadFromFile(std::string fileName_arg);
	};

	std::vector<Object*> objectPool;
	int objectPoolSize;
};

#endif