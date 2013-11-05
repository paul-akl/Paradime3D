#ifndef COMMON_CLASSES_NAMESPACE
#define COMMON_CLASSES_NAMESPACE

#include <string>
#include <vector>

#include <SDL.h>

#include "Config.h"
#include "Math.h"

namespace Common
{
	class ObjectParameters
	{
	public:

		ObjectParameters();

		bool modified;
		Math3d::Vec3f	positionVec,
						rotationVec,
						scaleVec;

		Math3d::Mat4f	modelMat,
						modelViewMat,
						modelViewProjMat;

	};
	class InputState
	{
	private:

		struct singleKey
		{
			singleKey()
			{
				name = ' ';
				scancode = SDL_Scancode(0);
				status = false;
			}
			void init(int *scancode_arg)
			{
				configScancode = scancode_arg;
				scancode = SDL_Scancode(*configScancode);
				name = *SDL_GetScancodeName(scancode);
				status = false;
			}
			void setScancode(int scancode_arg)
			{
				configScancode = &scancode_arg;
				scancode = SDL_Scancode(*configScancode);
				name = *SDL_GetScancodeName(scancode);
			}
			char name;
			SDL_Scancode scancode;
			bool status;
			int *configScancode;
		};
		
		void initKeyAssignments();

	public:

		singleKey	forwardKey,
					backwardKey,
					leftKey,
					rightKey, 
					jumpKey,
					fullscreenKey,
					vsyncKey,
					clipMouseKey,
					debug1Key,
					escKey,
					backKey,
					arrowUpKey,
					arrowDownKey,
					arrowLeftKey,
					arrowRightKey;

		int			mousePosX,
					mousePosY;

		bool		mouseEvent;

		InputState();
		void init();
	};
	class Perspective
	{
	public:

		Perspective()
		{
			modified = true;
		}

		bool modified;
		Math3d::Mat4f perspectiveMat;
	};
	class Cubemap
	{
	public:

		Cubemap();
		Cubemap(std::string XPosTextureName_arg, std::string XNegTextureName_arg,
				std::string YPosTextureName_arg, std::string YNegTextureName_arg,
				std::string ZPosTextureName_arg, std::string ZNegTextureName_arg);
		~Cubemap();
		
		std::string textureNames[6],
					cubemapName;
	};
}

#endif