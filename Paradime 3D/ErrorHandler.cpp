#include <iostream>
#include <sstream>

#include <SDL.h>
#include <Windows.h>

#include "Config.h"
#include "ErrorHandler.h"

namespace Message
{
	struct messageOrigin
	{
		int type;
		std::string message;
	};
	messageOrigin messageOrigins[13];
    int index = 0;
	
	messageCode::messageCode(int type_arg, int origin_arg, std::string message_arg)
	{
		type = type_arg;
		origin = origin_arg;
		message = message_arg;
		internalErrorCode = 0;
	}
	messageCode::messageCode(int type_arg, int origin_arg, std::string message_arg, int internalErrorCode_arg)
	{
		type = type_arg;
		origin = origin_arg;
		message = message_arg;
		internalErrorCode = internalErrorCode_arg;
	}

	void messageCode::display()
	{
		show(type, origin, message);
	}
	void setType(std::string message_arg)
	{
		messageOrigins[index].message = message_arg;
		messageOrigins[index].type = index;
		index++;
	}
	void show(int type_arg, int origin_arg, std::string message_arg)
	{
		std::string displayMessage;
		switch(type_arg)
		{
		case 0:
			displayMessage = messageOrigins[origin_arg].message + " Fatal Error: " + message_arg;
			MessageBox(NULL, displayMessage.c_str(), (Config::window::name + ": " + messageOrigins[origin_arg].message + " Error").c_str(), MB_OK | MB_ICONSTOP);
			SDL_Quit();
			exit(1);
			break;
		case 1:
			displayMessage = messageOrigins[origin_arg].message + " Error: " + message_arg;
			MessageBox(NULL, displayMessage.c_str(), (Config::window::name + ": " + messageOrigins[origin_arg].message + " Error").c_str(), MB_OK | MB_ICONSTOP);
			break;
		case 2:
			std::cout << displayMessage << messageOrigins[origin_arg].message << " Warning: " << message_arg << std::endl;
			break;
		case 3:
			std::cout << displayMessage << messageOrigins[origin_arg].message << ": " << message_arg << std::endl;
			break;
		}
	}
	void forceQuit()
	{
		Config::running = false;
		SDL_Quit();
		//exit(1);
	}
	void quit()
	{
		if(MessageBox(NULL, "Are You sure You want to quit?" , Config::window::name.c_str() , MB_YESNO | MB_ICONQUESTION) == 6)
			forceQuit();
	}
	void init()
	{
		setType("Engine");	
		setType("Configuration Parser");	
		setType("Texture Loader");	
		setType("Model Loader");
		setType("Shader Compiler");	
		setType("Game Object");
		setType("Player Manager");
		setType("Sky");
		setType("Lighting");
		setType("Geometry Buffer");
		setType("Scene Loader");
		setType("Renderer");
		setType("Config-File Loader");
	}

	std::string Message::toString(int int_arg)
	{
		std::stringstream stringstream_ret;
		stringstream_ret << int_arg;
		return stringstream_ret.str();
	}
	std::string Message::toString(unsigned int uint_arg)
	{
		std::stringstream stringstream_ret;
		stringstream_ret << uint_arg;
		return stringstream_ret.str();
	}
	std::string Message::toString(double double_arg)
	{
		std::stringstream stringstream_ret;
		stringstream_ret << double_arg;
		return stringstream_ret.str();
	}
	std::string Message::toString(float float_arg)
	{
		std::stringstream stringstream_ret;
		stringstream_ret << float_arg;
		return stringstream_ret.str();
	}
	std::string Message::toString(bool bool_arg)
	{
		return (bool_arg) ? "true" : "false";
	}
	std::string Message::toString(char char_arg)
	{
		std::string stringRet;
		stringRet += char_arg;
		return stringRet;
	}
}