#include <iostream>

#include "AdaptiveUniforms.h"

AdaptiveUniforms::AdaptiveUniforms(GLuint *shaderProgram_arg)
{
	numUpdatesPerFrame = 0;
	numUpdatesPerModel = 0;
	numUpdatesPerMesh = 0;
	numTextureUpdates = 0;

	generateUniformList(shaderProgram_arg);
}
AdaptiveUniforms::~AdaptiveUniforms()
{

}
	
void AdaptiveUniforms::generateUniformList(GLuint *shaderProgram_arg)
{
	std::vector<UniformFunctions::BaseUniformFunction*> updatesPerFrameVector, 
														updatesPerModelVector,
														updatesPerMeshVector,
														textureUniformUpdates;

	UniformFunctions::generatePerFrameList(&updatesPerFrameVector, shaderProgram_arg);
	UniformFunctions::generatePerModelList(&updatesPerModelVector, shaderProgram_arg);
	UniformFunctions::generatePerMeshList(&updatesPerModelVector, shaderProgram_arg);
	UniformFunctions::generateTextureUpdateList(&textureUniformUpdates, shaderProgram_arg);

	if(updatesPerFrameVector.size() > 0)
	{
		numUpdatesPerFrame = (updatesPerFrameVector.size());
		updatesPerFrame = new UniformFunctions::BaseUniformFunction*[numUpdatesPerFrame];

		for(int i=0; i < numUpdatesPerFrame; i++)
			updatesPerFrame[i] = updatesPerFrameVector[i];
	}

	if(updatesPerModelVector.size() > 0)
	{
		numUpdatesPerModel = (updatesPerModelVector.size());
		updatesPerModel = new UniformFunctions::BaseUniformFunction*[numUpdatesPerModel];
		
		for(int i=0; i < numUpdatesPerModel; i++)
			updatesPerModel[i] = updatesPerModelVector[i];
	}

	if(updatesPerMeshVector.size() > 0)
	{
		numUpdatesPerMesh = (updatesPerMeshVector.size());
		updatesPerMesh = new UniformFunctions::BaseUniformFunction*[numUpdatesPerMesh];
		
		for(int i=0; i < numUpdatesPerMesh; i++)
			updatesPerMesh[i] = updatesPerMeshVector[i];
	}
	
	if(textureUniformUpdates.size() > 0)
	{
		numTextureUpdates = (textureUniformUpdates.size());
		textureUpdates = new UniformFunctions::BaseUniformFunction*[numTextureUpdates];
		
		for(int i=0; i < numTextureUpdates; i++)
			textureUpdates[i] = textureUniformUpdates[i];
	}
}

void AdaptiveUniforms::updateFrame()
{
	for(int i=0; i < numUpdatesPerFrame; i++)
		updatesPerFrame[i]->updateUniform();
}
void AdaptiveUniforms::updateModel()
{
	for(int i=0; i < numUpdatesPerModel; i++)
		updatesPerModel[i]->updateUniform();
}
void AdaptiveUniforms::updateMesh()
{
	for(int i=0; i < numUpdatesPerMesh; i++)
		updatesPerMesh[i]->updateUniform();
}
void AdaptiveUniforms::updateTextureUniforms()
{
	for(int i=0; i < numTextureUpdates; i++)
		textureUpdates[i]->updateUniform();
}

int AdaptiveUniforms::getNumUpdatesPerFrame()
{
	return numUpdatesPerFrame;
}
int AdaptiveUniforms::getNumUpdatesPerModel()
{
	return numUpdatesPerModel;
}
int AdaptiveUniforms::getNumUpdatesPerMesh()
{
	return numUpdatesPerMesh;
}
int AdaptiveUniforms::getNumTextureUpdates()
{
	return numTextureUpdates;
}