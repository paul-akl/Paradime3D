#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <iostream>

#include "CurrentUpdateState.h"
#include "Loader_Model.h"

namespace ModelLoader
{
	std::vector<Model*> modelPool;

	GenericModel::GenericModel(std::string fileName_arg)
	{
		meshPoolSize = 0;

		try
		{
			load(fileName_arg);
			loadBuffer();
		}
		catch(Message::messageCode error)
		{
			error.display();
		}

		meshPoolSize = meshPool.size();
	}
	GenericModel::~GenericModel()
	{

	}

	void GenericModel::load(std::string fileName_arg)
	{
		Assimp::Importer assimpImporter;
		modelName = fileName_arg;

		const aiScene* assimpScene = assimpImporter.ReadFile(Config::path::models_path + modelName, 
			aiProcess_GenSmoothNormals | aiProcess_ValidateDataStructure | aiProcess_FindInvalidData | aiProcess_Triangulate);

		if(!assimpScene)
			throw Message::messageCode(MSG_ERROR, MSG_MODEL, modelName + " failed to load (" + assimpImporter.GetErrorString() + ").");

		loadFromScene(assimpScene);
	}
	void GenericModel::loadBuffer()
	{
		// Create and bind the Vertex Array Object
		glGenVertexArrays(1, &VAO);   
		glBindVertexArray(VAO);

		// Create the buffers
		glGenBuffers(sizeof(buffers)/sizeof(buffers[0]), buffers);

  		glBindBuffer(GL_ARRAY_BUFFER, buffers[POSITION_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);    
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
		
		// Make sure the VAO is not changed from the outside
		glBindVertexArray(0);
	}
	void GenericModel::render(ShaderLoader::BaseShader *shader_arg)
	{
		// Bind Vertex Array Object
		glBindVertexArray(VAO);

		// Make sure the correct shader is being used
		shader_arg->bind();

		// Update the uniforms for this model
		shader_arg->updateModel();

		// Render each mesh
		for (int i=0; i < meshPoolSize; i++) 
		{
			// Update the uniforms required by each draw call
			shader_arg->updateMesh();

			glDrawElementsBaseVertex(GL_TRIANGLES, meshPool[i].numIndices, GL_UNSIGNED_INT,
				(void*)(sizeof(unsigned int) * meshPool[i].baseIndex), meshPool[i].baseVertex);
		}
	}

	void GenericModel::loadFromScene(const aiScene* assimpScene_arg)
	{
		// Reserve space in the vectors for every mesh
		meshPool.resize(assimpScene_arg->mNumMeshes);
		
		unsigned int numVerticesTotal = 0;
		unsigned int numIndicesTotal = 0;

		// Count the number of vertices and indices
		for (unsigned int i = 0 ; i < assimpScene_arg->mNumMeshes ; i++) 
		{       
		    meshPool[i].numIndices = assimpScene_arg->mMeshes[i]->mNumFaces * 3;
		    
		    meshPool[i].baseVertex = numVerticesTotal;
		    meshPool[i].baseIndex = numIndicesTotal;

		    numVerticesTotal += assimpScene_arg->mMeshes[i]->mNumVertices;
		    numIndicesTotal  += meshPool[i].numIndices;
		}
		
		// Reserve space in the vectors for the vertex attributes and indices
		positions.reserve(numVerticesTotal);
		indices.reserve(numIndicesTotal);

		// Deal with each mesh and texture
		for(unsigned int i=0; i < assimpScene_arg->mNumMeshes; i++)
			loadFromMesh(assimpScene_arg->mMeshes[i]);
	}
	void GenericModel::loadFromMesh(const aiMesh* assimpMesh_arg)
	{
		// Put the vertex positions, normals and texture coordinate data from assimp to memory
		for(unsigned int i=0; i < assimpMesh_arg->mNumVertices; i++)
		{
			positions.push_back(Math3d::Vec3f(assimpMesh_arg->mVertices[i].x, assimpMesh_arg->mVertices[i].y, assimpMesh_arg->mVertices[i].z));
		}

		// Put the indices data from assimp to memory
		for(unsigned int i=0; i < assimpMesh_arg->mNumFaces; i++)
		{
			if(assimpMesh_arg->mFaces[i].mNumIndices == 3)
			{
				indices.push_back(assimpMesh_arg->mFaces[i].mIndices[0]);
				indices.push_back(assimpMesh_arg->mFaces[i].mIndices[1]);
				indices.push_back(assimpMesh_arg->mFaces[i].mIndices[2]);
			}
		}
	}

	Model::Model(std::string fileName_arg)
	{
		try
		{
			load(fileName_arg);
			loadBuffer();
		}
		catch(Message::messageCode error)
		{
			error.display();
		}

		meshPoolSize = 0;
		meshPoolSize = meshPool.size();
	}
	Model::~Model()
	{
		Message::show(MSG_INFO, MSG_MODEL, modelName + " has been removed from memory.");
	}

	void Model::load(std::string fileName_arg)
	{
		Assimp::Importer assimpImporter;
		modelName = fileName_arg;
		pointerCounter = 1;

		pHandle * handle;

		handle = new pHandle();

		assimpImporter.SetProgressHandler(handle);

		const aiScene* assimpScene = assimpImporter.ReadFile(Config::path::models_path + modelName, 
			aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_SortByPType);

		if(!assimpScene)
			throw Message::messageCode(MSG_ERROR, MSG_MODEL, modelName + " failed to load (" + assimpImporter.GetErrorString() + ").");

		loadFromScene(assimpScene);
	}
	void Model::unload()
	{
		for(unsigned int i=0; i < meshPool.size(); i++)
		{
			//meshPool[i].positions.clear();
			//meshPool[i].normals.clear();
			//meshPool[i].texCoords.clear();
			//meshPool[i].indices.clear();
			//meshPool[i].materialIndex.clear();
		}
	}
	void Model::loadBuffer()
	{
		// Create and bind the Vertex Array Object
		glGenVertexArrays(1, &VAO);   
		glBindVertexArray(VAO);

		// Create the buffers
		glGenBuffers(sizeof(buffers)/sizeof(buffers[0]), buffers);

  		glBindBuffer(GL_ARRAY_BUFFER, buffers[POSITION_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);    

		if(texCoords.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, buffers[TEXCOORD_BUFFER]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if(normals.size() > 0)
		{
   			glBindBuffer(GL_ARRAY_BUFFER, buffers[NORMAL_BUFFER]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

		// Make sure the VAO is not changed from the outside
		glBindVertexArray(0);
	}
	void Model::unloadBuffer()
	{
		
	}
	void Model::loadFromScene(const aiScene* assimpScene_arg)
	{
		// Reserve space in the vectors for every mesh
		meshPool.resize(assimpScene_arg->mNumMeshes);

		unsigned int numVerticesTotal = 0;
		unsigned int numIndicesTotal = 0;

		// Count the number of vertices and indices
		for (unsigned int i=0 ; i < assimpScene_arg->mNumMeshes ; i++) 
		{       
		    meshPool[i].materialIndex = assimpScene_arg->mMeshes[i]->mMaterialIndex;
		    meshPool[i].numIndices = assimpScene_arg->mMeshes[i]->mNumFaces * 3;
		    
		    meshPool[i].baseVertex = numVerticesTotal;
		    meshPool[i].baseIndex = numIndicesTotal;

		    numVerticesTotal += assimpScene_arg->mMeshes[i]->mNumVertices;
		    numIndicesTotal  += meshPool[i].numIndices;
		}
		
		// Reserve space in the vectors for the vertex attributes and indices
		positions.reserve(numVerticesTotal);
		normals.reserve(numVerticesTotal);
		texCoords.reserve(numVerticesTotal);
		indices.reserve(numIndicesTotal);

		// Deal with each mesh and texture
		for(unsigned int i=0; i < assimpScene_arg->mNumMeshes; i++)
			loadFromMesh(assimpScene_arg->mMeshes[i]);
		
		materialPool = new MaterialPool(assimpScene_arg);
	}
	void Model::loadFromMesh(const aiMesh* assimpMesh_arg)
	{
		// Make sure that the texture coordinates array exist (by checking if the first member of the array does)
		bool textureCoordsExist = assimpMesh_arg->mTextureCoords[0] ? true : false;

		// Put the vertex positions, normals and texture coordinate data from assimp to memory
		for(unsigned int i=0; i < assimpMesh_arg->mNumVertices; i++)
		{
			positions.push_back(Math3d::Vec3f(assimpMesh_arg->mVertices[i].x, assimpMesh_arg->mVertices[i].y, assimpMesh_arg->mVertices[i].z));
			//if(assimpMesh_arg->
			normals.push_back(Math3d::Vec3f(assimpMesh_arg->mNormals[i].x, assimpMesh_arg->mNormals[i].y, assimpMesh_arg->mNormals[i].z));
			if(textureCoordsExist)
				texCoords.push_back(Math3d::Vec2f(assimpMesh_arg->mTextureCoords[0][i].x, assimpMesh_arg->mTextureCoords[0][i].y));
		}

		// Put the indices data from assimp to memory
		for(unsigned int i=0; i < assimpMesh_arg->mNumFaces; i++)
		{
			if(assimpMesh_arg->mFaces[i].mNumIndices == 3)
			{
				indices.push_back(assimpMesh_arg->mFaces[i].mIndices[0]);
				indices.push_back(assimpMesh_arg->mFaces[i].mIndices[1]);
				indices.push_back(assimpMesh_arg->mFaces[i].mIndices[2]);
			}
		}
	}
	std::string Model::loadFromMaterial(const aiMaterial* assimpMaterial_arg)
	{
		aiString materialPath;

		if (assimpMaterial_arg->GetTexture(aiTextureType_DIFFUSE, 0, &materialPath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
		{
			return materialPath.data;
		}
		else
		{
			std::string errorMessage = materialPath.data;
			Message::show(MSG_WARNING, MSG_TEXTURE, errorMessage + " has failed to load.");

			return Config::texture::default_texture;
		}
	}
	void Model::render(std::vector<TextureLoader::Texture2D*> *texturePool_arg, ShaderLoader::BaseShader *shader_arg)
	{
		// Bind Vertex Array Object
		glBindVertexArray(VAO);

		// Make sure the correct shader is being used
		shader_arg->bind();

		// Update the uniforms for this model
		shader_arg->updateModel();

		// Render each mesh
		for (int i=0; i < meshPoolSize; i++)
		{
			// To avoid fatal error, check if there are enough materials in the vector, before binding
			//if(meshPool[i].materialIndex < (*texturePool_arg).size())
			(*texturePool_arg)[meshPool[i].materialIndex]->bind();

			// Update the uniforms required by each draw call
			shader_arg->updateMesh();

			glDrawElementsBaseVertex(GL_TRIANGLES, meshPool[i].numIndices, GL_UNSIGNED_INT,
				(void*)(sizeof(unsigned int) * meshPool[i].baseIndex), meshPool[i].baseVertex);
		}
		
		glBindVertexArray(0);
		/*GLenum glError = glGetError();
		if(glError)
			std::cout << "GLEW ERROR " << glError << ": " << glewGetErrorString(glError) << std::endl;*/
	}
	bool Model::compareName(std::string *modelName_arg)
	{
		return (*modelName_arg == modelName);
	}

	Model::MaterialPool::MaterialPool(const aiScene* assimpScene_arg)
	{
		numMaterials = 0;
		loadFromScene(assimpScene_arg);
	}
	Model::MaterialPool::~MaterialPool()
	{

	}
			
	void Model::MaterialPool::loadFromScene(const aiScene* assimpScene_arg)
	{
		numMaterials = assimpScene_arg->mNumMaterials;

		aiString materialPath;
		diffuseMaterials.resize(numMaterials);
		for(unsigned int i=0; i < assimpScene_arg->mNumMaterials; i++)
		{
			assimpScene_arg->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &materialPath, NULL, NULL, NULL, NULL, NULL);
			diffuseMaterials[i] = materialPath.data;
		}
	}

	void init()
	{
		Message::show(MSG_INFO, MSG_MODEL, "has beed initialized.");
	}

	Model *load(std::string modelName_arg)
	{
		for(unsigned int i=0; i < modelPool.size(); i++)
		{
			if(modelPool[i]->compareName(&modelName_arg))
			{
				modelPool[i]->pointerCounter++;
				Message::show(MSG_INFO, MSG_MODEL, modelName_arg + " pointer counter set to " + Message::toString(modelPool[i]->pointerCounter));

				return modelPool[i];
			}
		}
		modelPool.push_back(new Model(modelName_arg));
		return modelPool[modelPool.size() - 1];
	}
	void unload(std::string modelName_arg)
	{
		for(unsigned int i=0; i < modelPool.size(); i++)
		{
			if(modelPool[i]->compareName(&modelName_arg))
			{
				modelPool[i]->pointerCounter--;

				if(modelPool[i]->pointerCounter < 1)
				{
					delete modelPool[i];
					modelPool.erase(modelPool.begin() + i);
				}
				else
				{
					Message::show(MSG_INFO, MSG_MODEL, modelName_arg + " pointer counter set to " + Message::toString(modelPool[i]->pointerCounter));
				}
				break;
			}
		}
	}
}