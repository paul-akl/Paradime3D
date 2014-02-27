#include <iostream>

#include "CommonClasses.h"
#include "ErrorHandler.h"

namespace Common
{
	ObjectParameters::ObjectParameters()
	{
		modified = true;
		scaleVec = Math3d::Vec3f(1, 1, 1);
	}
	void ObjectParameters::calcModelMat()
	{
		if(modified)
		{
			modified = false;
			modelMat.identity();
			modelMat.rotate(rotationVec);
			modelMat.transform(positionVec);
			modelMat.scale(scaleVec);
		}
	}

	InputState::InputState()
	{
		mousePosX = 0;
		mousePosY = 0;
	}
	void InputState::init()
	{
		initKeyAssignments();
	}
	void InputState::initKeyAssignments()
	{
		forwardKey.init(&Config::keys::forward);
		backwardKey.init(&Config::keys::backward);
		leftKey.init(&Config::keys::left);
		rightKey.init(&Config::keys::right);
		jumpKey.init(&Config::keys::jump);
		fullscreenKey.init(&Config::keys::fullscreen);
		vsyncKey.init(&Config::keys::vsync);
		clipMouseKey.init(&Config::keys::clip_mouse);
		debug1Key.init(&Config::keys::debug_1);
		debug2Key.init(&Config::keys::debug_2);
		escKey.init(&Config::keys::escKey);
		backKey.init(&Config::keys::backKey);
		arrowUpKey.init(&Config::keys::arrow_up);
		arrowDownKey.init(&Config::keys::arrow_down);
		arrowLeftKey.init(&Config::keys::arrow_left);
		arrowRightKey.init(&Config::keys::arrow_right);
	}

	Cubemap::Cubemap()
	{
		textureNames[0] = Config::texture::default_texture;
		textureNames[1] = Config::texture::default_texture;
		textureNames[2] = Config::texture::default_texture;
		textureNames[3] = Config::texture::default_texture;
		textureNames[4] = Config::texture::default_texture;
		textureNames[5] = Config::texture::default_texture;
	}
	Cubemap::Cubemap(std::string XPosTextureName_arg, std::string XNegTextureName_arg,
					 std::string YPosTextureName_arg, std::string YNegTextureName_arg,
					 std::string ZPosTextureName_arg, std::string ZNegTextureName_arg)
	{
		textureNames[0] = XPosTextureName_arg;
		textureNames[1] = XNegTextureName_arg;
		textureNames[2] = YPosTextureName_arg;
		textureNames[3] = YNegTextureName_arg;
		textureNames[4] = ZPosTextureName_arg;
		textureNames[5] = ZNegTextureName_arg;
	}
	Cubemap::~Cubemap()
	{

	}
}