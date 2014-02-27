#ifndef LOADER_MODEL_NAMESPACE
#define LOADER_MODEL_NAMESPACE

#include <string>
#include <vector>

#include <assimp\Importer.hpp>
#include <assimp\ProgressHandler.hpp>
#include <assimp\scene.h>
#include <GL/glew.h>
#include <SDL.h>

#include "AdaptiveUniforms.h"
#include "CommonClasses.h"
#include "Config.h"
#include "ErrorHandler.h"
#include "Loader_Shader.h"
#include "Loader_Texture.h"
#include "Math.h"

namespace ModelLoader
{
	class GenericModel
	{
	public:
		GenericModel(std::string fileName_arg);
		~GenericModel();

		void load(std::string fileName_arg);
		void loadBuffer();
		void render(ShaderLoader::BaseShader *shader_arg);

		std::string modelName;

	private:
		  
		#define POSITION_BUFFER 1
		#define INDEX_BUFFER	0  

		struct Mesh
		{
			Mesh()
			{
				numIndices = 0;
				baseVertex = 0;
				baseIndex = 0;
			}
			
			unsigned int numIndices;
			unsigned int baseVertex;
			unsigned int baseIndex;
		};
		
		int meshPoolSize;
		
		GLuint VAO, buffers[2];

		std::vector<Mesh> meshPool;

		std::vector<Math3d::Vec3f> positions;
		std::vector<unsigned int> indices;
		
		void loadFromScene(const aiScene* assimpScene_arg);
		void loadFromMesh(const aiMesh* assimpMesh_arg);
	};

	class Model
	{
	public:

		class MaterialPool
		{
		public:

			MaterialPool(const aiScene* assimpScene_arg);
			~MaterialPool();
			
			std::vector<std::string> diffuseMaterials;
			unsigned int numMaterials;

		private:

			void loadFromScene(const aiScene* assimpScene_arg);

		};
		
		class pHandle : public Assimp::ProgressHandler
		{
		public:
			int count;
			pHandle()
			{
				count = 0;
			}

			~pHandle()
			{

			}
			bool Update(float percentage)
			{
				count++;
				return false;
			}
		};

		Model(std::string fileName_arg);
		~Model();

		void load(std::string fileName_arg);
		void unload();

		void loadBuffer();
		void unloadBuffer();

		void render(std::vector<TextureLoader::Texture2D*> *texturePool_arg, ShaderLoader::BaseShader *shader_arg);
		bool compareName(std::string *modelName_arg);

		unsigned int pointerCounter, numMaterials;
		
		MaterialPool *materialPool;
		std::string modelName;

	private:

		#define INDEX_BUFFER	0    
		#define POSITION_BUFFER 1
		#define NORMAL_BUFFER	2
		#define TEXCOORD_BUFFER 3

		struct Mesh
		{
			Mesh()
			{
				numIndices = 0;
				baseVertex = 0;
				baseIndex = 0;
				materialIndex = 0;
			}
			
			int numIndices;
			int baseVertex;
			int baseIndex;
			int materialIndex;
		};
		
		void loadFromScene(const aiScene* assimpScene_arg);
		void loadFromMesh(const aiMesh* assimpMesh_arg);
		std::string loadFromMaterial(const aiMaterial* assimpMaterial_arg);
		
		int meshPoolSize;
		
		GLuint VAO, buffers[4];

		std::vector<Mesh> meshPool;
		std::vector<std::string> materialNames;

		std::vector<Math3d::Vec3f> positions;
		std::vector<Math3d::Vec3f> normals;
		std::vector<Math3d::Vec2f> texCoords;
		std::vector<unsigned int> indices;
	};
	
	void init();

	Model *load(std::string modelName_arg);
	void unload(std::string modelName_arg);
}

#endif