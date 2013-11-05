#ifndef LOADER_TEXTURE_NAMESPACE
#define LOADER_TEXTURE_NAMESPACE

#include <string>
#include <vector>

#include <GL/glew.h>

#include "CommonClasses.h"

namespace TextureLoader
{
	class Texture2D
	{
	public:

		unsigned int pointerCounter;
		std::string fileName;
		GLuint textureHandle;

		Texture2D(std::string fileName_arg, GLuint textureHandle_arg);
		~Texture2D();

		bool compareFileName(std::string fileName_arg);
		void bind();
	};
	class Texture3D
	{
	public:

		unsigned int pointerCounter;
		Common::Cubemap fileNames;
		GLuint textureHandle;

		Texture3D(Common::Cubemap *fileNames_arg, GLuint textureHandle_arg);
		~Texture3D();

		bool compareFileName(Common::Cubemap *cubemap_arg);
		void bind();
	};
	
	GLuint load2DFromFile(std::string fileName_arg);
	GLuint load3DFromFile(Common::Cubemap *cubemap_arg);

	Texture2D *load2D(std::string fileName_arg);
	Texture2D *load2Ddefault();
	void unload2D(std::string fileName_arg);

	Texture3D *load3D(Common::Cubemap *cubemap_arg);
	void unload3D(Common::Cubemap *cubemap_arg);

	void init();
}

#endif