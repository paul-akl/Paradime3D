#ifndef ERROR_HANDLER_CLASS
#define ERROR_HANDLER_CLASS
#ifdef _WIN32

#include <string>
//#include <Windows.h>

#include <GL/glew.h>

#define MSG_FATAL_ERROR 0
#define MSG_ERROR		1
#define MSG_WARNING		2
#define MSG_INFO		3

#define MSG_ENGINE	 0
#define MSG_CONFIG	 1
#define MSG_TEXTURE  2
#define MSG_MODEL	 3
#define MSG_SHADER	 4
#define MSG_OBJECT	 5
#define MSG_PLAYER	 6
#define MSG_SKYBOX	 7
#define MSG_LIGHT	 8
#define MSG_GBUFFER	 9
#define MSG_SCENE	 10
#define MSG_RENDERER 11

namespace Message
{	
	class messageCode
	{
	public:
		int type;
		int origin;
		int internalErrorCode;
		std::string message;

		messageCode(int type_arg, int origin_arg, std::string message_arg);
		messageCode(int type_arg, int origin_arg, std::string message_arg, int internalErrorCode_arg);
		void display();
	};

	void setType(std::string message_arg);
	void init();

	void show(int type_arg, int origin_arg, std::string message_arg);
	void forceQuit();
	void quit();

	std::string toString(int int_arg);
	std::string toString(unsigned int uint_arg);
	std::string toString(double double_arg);
	std::string toString(float float_arg);
	std::string toString(bool bool_arg);
}

#endif
#endif