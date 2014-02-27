#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include <FreeImage.h>

#include "Config.h"
#include "CurrentUpdateState.h"
#include "ErrorHandler.h"
#include "Loader_Texture.h"

namespace TextureLoader
{
	std::vector<Texture2D*> texture2DPool;
	std::vector<Texture3D*> texture3DPool;
	GLuint *currentTexture,
			emptyTextureHandle;

	static const GLenum texture3DTypes[6] = {	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
												GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
												GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
												GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
												GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
												GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

	Texture2D::Texture2D(std::string fileName_arg, GLuint textureHandle_arg)
	{
		fileName = fileName_arg;
		textureHandle = textureHandle_arg;

		pointerCounter = 1;

		//specularPower = (specularPower_arg == 0) ? Config::texture::default_specular_power : specularPower_arg;
		//specularIntensity = (specularIntensity_arg == 0) ? Config::texture::default_specular_intensity : specularIntensity_arg;

		Message::show(MSG_INFO, MSG_TEXTURE, fileName + " 2D texture handle created.");
	}
	Texture2D::~Texture2D()
	{
		Message::show(MSG_INFO, MSG_TEXTURE, fileName + " has been removed from memory.");
	}

	bool Texture2D::compareFileName(std::string fileName_arg)
	{
		return (fileName_arg == fileName);
	}
	void Texture2D::bind()
	{
		//if(textureHandle != *currentTexture)
		//{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureHandle);
		//}
	}
	void Texture2D::bind(int activeTextureUnit_arg)
	{
		glActiveTexture(GL_TEXTURE0 + activeTextureUnit_arg);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
	}
	void Texture2D::setWrap(int wrapType_arg)
	{
		switch(wrapType_arg)
		{
		case Repeat:
			glBindTexture(GL_TEXTURE_2D, textureHandle);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;
			
		case ClampToEdge:
			glBindTexture(GL_TEXTURE_2D, textureHandle);
			//bind();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;
			
		case ClampToBorder:
			glBindTexture(GL_TEXTURE_2D, textureHandle);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			break;
			
		case MirroredRepeat:
			glBindTexture(GL_TEXTURE_2D, textureHandle);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			break;
		}
	}

	Texture3D::Texture3D(Common::Cubemap *fileNames_arg, GLuint textureHandle_arg)
	{
		fileNames = *fileNames_arg;
		textureHandle = textureHandle_arg;
		pointerCounter = 1;

		Message::show(MSG_INFO, MSG_TEXTURE, fileNames.cubemapName + " 3D texture handle created.");
	}
	Texture3D::~Texture3D()
	{

	}

	bool Texture3D::compareFileName(Common::Cubemap *cubemap_arg)
	{
		return (fileNames.cubemapName == cubemap_arg->cubemapName);
	}
	void Texture3D::bind()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandle);
	}

	Texture2D *load2D(std::string fileName_arg)
	{
		// Check if a texture is already loaded, return it
		for(unsigned int i=0; i < texture2DPool.size(); i++)
		{
			if(texture2DPool[i]->compareFileName(fileName_arg))
			{
				texture2DPool[i]->pointerCounter++;
				Message::show(MSG_INFO, MSG_TEXTURE, fileName_arg + " pointer counter set to " + Message::toString(texture2DPool[i]->pointerCounter));
				return texture2DPool[i];
			}
		}

		// If a texture is not loaded, load it from a file and return it
		GLuint tempTexture;
		try
		{
			tempTexture = load2DFromFile(fileName_arg);
			texture2DPool.push_back(new Texture2D(fileName_arg, tempTexture));
			return texture2DPool[texture2DPool.size() - 1];
		}
		catch(Message::messageCode error)
		{
			error.display();
			try
			{
				for(unsigned int i=0; i < texture2DPool.size(); i++)
				{
					if(texture2DPool[i]->compareFileName(Config::texture::default_texture))
					{
						texture2DPool[i]->pointerCounter++;
						Message::show(MSG_INFO, MSG_TEXTURE, Config::texture::default_texture + " pointer counter set to " + Message::toString(texture2DPool[i]->pointerCounter));
						return texture2DPool[i];
					}
				}
				
				tempTexture = load2DFromFile(Config::texture::default_texture);
				texture2DPool.push_back(new Texture2D(Config::texture::default_texture, tempTexture));
				return texture2DPool[texture2DPool.size() - 1];
			}
			catch(Message::messageCode error)
			{
				Message::show(MSG_ERROR, MSG_TEXTURE, "Default texture has failed to load.");
			}
		}
	}
	Texture2D *load2Ddefault()
	{
		return load2D(Config::texture::default_texture);
	}
	GLuint load2DFromFile(std::string fileName_arg)
	{
		GLuint TextureID = NULL;																							//Creating container for the Texture2D
		FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType((Config::path::textures_path + fileName_arg).c_str(), 0);		//Reading the format of the image
		FIBITMAP* bitmap = FreeImage_Load(imageFormat, (Config::path::textures_path + fileName_arg).c_str());				//Reading the actual image

		FIBITMAP* temp = bitmap;
		bitmap = FreeImage_ConvertTo32Bits(bitmap);
		FreeImage_Unload(temp);
		
		int imageWidth = FreeImage_GetWidth(bitmap);
		int imageHeight = FreeImage_GetHeight(bitmap);

		if(bitmap)
		{
			Message::show(MSG_INFO, MSG_TEXTURE, fileName_arg + " (" + Message::toString(imageWidth) + "x" + Message::toString(imageHeight) + ") has been loaded.");
			
			GLubyte* Texture2D = new GLubyte[4*imageWidth*imageHeight];
			char* Pixels = (char*)FreeImage_GetBits(bitmap);
	
			//FreeImage loads in BGR format, therefore swap of bytes is needed (Or usage of GL_BGR).
			for(int i = 0; i<imageWidth*imageHeight; i++)
			{
				Texture2D[i * 4 + 0] = Pixels[i * 4 + 2];
				Texture2D[i * 4 + 1] = Pixels[i * 4 + 1];
				Texture2D[i * 4 + 2] = Pixels[i * 4 + 0];
				Texture2D[i * 4 + 3] = Pixels[i * 4 + 3];
			}
	
			//Generating the actual OpenGL Texture2D
			glGenTextures(1, &TextureID);
			glBindTexture(GL_TEXTURE_2D, TextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)Texture2D);
			if(Config::texture::generate_mipmaps)
				glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Config::texture::gl_texture_minification);	//Texture2D filtering mode, when image is minimized
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Config::texture::gl_texture_magnification);	//Texture2D filtering mode, when image is magnified
			
			GLfloat TextureAnisotropicFilter;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &TextureAnisotropicFilter);						// Get the maximum anisotropic filtering available
			if(Config::texture::gl_texture_anisotropy < TextureAnisotropicFilter)							// Check if the current anisotropic filtering value
				TextureAnisotropicFilter = (float)Config::texture::gl_texture_anisotropy;					// does not exceed the maximum available
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (int)TextureAnisotropicFilter);	// Texture2D anisitropic filtering
		}
		else
		{
			throw Message::messageCode(MSG_WARNING, MSG_TEXTURE, fileName_arg + " has failed to load.");
		}

		return TextureID;
	}
	void unload2D(std::string fileName_arg)
	{
		// Go through the whole texture2DPool vector to find a match
		for(unsigned int i=0; i < texture2DPool.size(); i++)
		{
			// If a match is found, lower the pointer counter. If it is lower than 1 (no object is using a texture), delete it
			if(texture2DPool[i]->compareFileName(fileName_arg))
			{
				texture2DPool[i]->pointerCounter--;
				if(texture2DPool[i]->pointerCounter < 1)
				{
					delete texture2DPool[i];
					texture2DPool.erase(texture2DPool.begin() + i);
				}
				else
				{
					Message::show(MSG_INFO, MSG_TEXTURE, fileName_arg + " pointer counter set to " + Message::toString(texture2DPool[i]->pointerCounter));
				}
				break;
			}
		}
	}
	
	Texture3D *load3D(Common::Cubemap *cubemap_arg)
	{
		// Check if a texture is already loaded, return it
		for(unsigned int i=0; i < texture3DPool.size(); i++)
		{
			if(texture3DPool[i]->compareFileName(cubemap_arg))
			{
				texture3DPool[i]->pointerCounter++;
				Message::show(MSG_INFO, MSG_TEXTURE, cubemap_arg->cubemapName + " pointer counter set to " + Message::toString(texture3DPool[i]->pointerCounter));
				return texture3DPool[i];
			}
		}

		// If a texture is not loaded, load it from a file and return it
		GLuint tempTexture;
		try
		{
			tempTexture = load3DFromFile(cubemap_arg);
			texture3DPool.push_back(new Texture3D(cubemap_arg, tempTexture));
			return texture3DPool[texture3DPool.size() - 1];
		}
		catch(Message::messageCode error)
		{
			if(cubemap_arg->textureNames[error.internalErrorCode] == Config::texture::default_texture)
			{
				Message::show(MSG_ERROR, MSG_TEXTURE, "Default texture has failed to load.");
			}
			else
			{
				error.display();

				cubemap_arg->textureNames[error.internalErrorCode] = Config::texture::default_texture;

				return load3D(cubemap_arg);
			}
		}
		return NULL; // Should never reach this, added to avoid a warning
	}
	GLuint load3DFromFile(Common::Cubemap *cubemap_arg)
	{
		GLuint TextureID = NULL;	

		for(int i=0; i < 6; i++)
		{																									//Creating container for the Texture2D
			FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType((Config::path::textures_path + cubemap_arg->textureNames[i]).c_str(), 0);		//Reading the format of the image
			FIBITMAP* bitmap = FreeImage_Load(imageFormat, (Config::path::textures_path + cubemap_arg->textureNames[i]).c_str());				//Reading the actual image
			
			FIBITMAP* temp = bitmap;
			bitmap = FreeImage_ConvertTo32Bits(bitmap);
			FreeImage_Unload(temp);
		
			int imageWidth = FreeImage_GetWidth(bitmap);
			int imageHeight = FreeImage_GetHeight(bitmap);

			if(bitmap)
			{
				Message::show(MSG_INFO, MSG_TEXTURE, cubemap_arg->textureNames[i] + " (" + Message::toString(imageWidth) + "x" + Message::toString(imageHeight) + ") has been loaded.");
			
				GLubyte* Texture2D = new GLubyte[4*imageWidth*imageHeight];
				char* Pixels = (char*)FreeImage_GetBits(bitmap);
	
				//FreeImage loads in BGR format, therefore swap of bytes is needed (Or usage of GL_BGR).
				for(int j = 0; j < imageWidth*imageHeight; j++){
					Texture2D[j * 4 + 0] = Pixels[j * 4 + 2];
					Texture2D[j * 4 + 1] = Pixels[j * 4 + 1];
					Texture2D[j * 4 + 2] = Pixels[j * 4 + 0];
					Texture2D[j * 4 + 3] = Pixels[j * 4 + 3];
				}
	
				//Generating the actual OpenGL Texture2D
				glGenTextures(1, &TextureID);
				glBindTexture(GL_TEXTURE_2D, TextureID);
				glTexImage2D(texture3DTypes[i],0,GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid*)Texture2D);
				
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, Config::texture::gl_texture_minification);				//Texture2D filtering mode, when image is minimized
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, Config::texture::gl_texture_magnification);				//Texture2D filtering mode, when image is magnified
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, Config::texture::gl_texture_anisotropy);		//Texture2D anisitropic filtering
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			}
			else
			{
				throw Message::messageCode(MSG_WARNING, MSG_TEXTURE, cubemap_arg->textureNames[i] + " has failed to load.", i);
			}

		}
		return TextureID;
	}
	void unload3D(Common::Cubemap cubemap_arg)
	{

	}

	void init()
	{
		currentTexture = &emptyTextureHandle;
		
		Message::show(MSG_INFO, MSG_TEXTURE, " has been initialized.");
	}
}