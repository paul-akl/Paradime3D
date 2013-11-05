#ifndef ADAPTIVE_UNIFORMS_CLASS
#define ADAPTIVE_UNIFORMS_CLASS

#include <GL/glew.h>

#include "AdaptiveUniformsFunctions.h"
#include "CommonClasses.h"

class AdaptiveUniforms
{

public:

	AdaptiveUniforms(GLuint *shaderProgram_arg);
	~AdaptiveUniforms();

	void generateUniformList(GLuint *shaderProgram_arg);

	void updateFrame();
	void updateModel();
	void updateMesh();
	void updateTextureUniforms();

	int getNumUpdatesPerFrame();
	int getNumUpdatesPerModel();
	int getNumUpdatesPerMesh();
	int getNumTextureUpdates();

private:
	
	int numUpdatesPerFrame,
		numUpdatesPerModel,
		numUpdatesPerMesh,
		numTextureUpdates;

	UniformFunctions::BaseUniformFunction	**updatesPerFrame,
											**updatesPerModel,
											**updatesPerMesh,
											**textureUpdates;
};

#endif