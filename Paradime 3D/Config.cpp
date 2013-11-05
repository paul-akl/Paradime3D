#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Config.h"
#include "ErrorHandler.h"
//#include "Game.h"

namespace Config
{
	std::string configFileName = "config.ini";
	int currentIndex = 0;
	const int numbOfParam = 73;
	
	bool running = true, drawDebugBuffers = false;
	int currentScreenWidth, currentScreenHeight;

	namespace window
	{
		std::string name;
		int position_x;
		int position_y;
		int size_windowed_x;
		int size_windowed_y;
		int size_fullscreen_x;
		int size_fullscreen_y;
		int default_display;
		bool fullscreen;
		bool mouse_clipped;
	}
	namespace engine
	{
		int delta_time_divider;
		int gl_context_major_version;
		int gl_context_minor_version;
		int multisample_buffers;
		int multisample_samples;
		int face_culling_mode;
		int depth_test_func;
		bool multisampling;
		bool depth_test;
		bool face_culling;
		bool vertical_sync;
		float z_far;
		float z_near;
		std::string stencil_pass_vert_shader;
		std::string stencil_pass_frag_shader;
	}
	namespace renderer
	{
		std::string dir_light_vert_shader;
		std::string dir_light_frag_shader;
		std::string point_light_vert_shader;
		std::string point_light_frag_shader;
		std::string spot_light_vert_shader;
		std::string spot_light_frag_shader;
		std::string dir_light_quad;
		std::string point_light_sphere;
		std::string stencil_pass_vert_shader;
		std::string stencil_pass_frag_shader;
		float dir_light_quad_offset_x;
		float dir_light_quad_offset_y;
		float dir_light_quad_offset_z;
		float dir_light_quad_rotation_x;
		float dir_light_quad_rotation_y;
		float dir_light_quad_rotation_z;
	}
	namespace texture
	{
		std::string default_texture;
		GLenum gl_texture_minification;
		GLenum gl_texture_magnification;
		int gl_texture_anisotropy;
		float default_specular_power;
		float default_specular_intensity;
	}
	namespace sound
	{
			
	}
	namespace path
	{
		std::string textures_path;
		std::string models_path;
		std::string shader_path;
		std::string sounds_path;
		std::string maps_path;
		std::string objects_path;
	}
	namespace keys
	{
		int forward;
		int backward;
		int left;
		int right;
		int jump;
		int fullscreen;
		int vsync;
		int clip_mouse;
		int debug_1;
		int escKey;
		int backKey;
		int arrow_up;
		int arrow_down;
		int arrow_left;
		int arrow_right;
		float mouse_sensitivity;
	}
	namespace game
	{
		float camera_freelook_speed;
	}
	namespace shader
	{
		std::string positionVector;
		std::string worldViewProjection;
	}
	namespace graphics
	{
		float FOV;
		int pointlights_max;
		int spotlights_max;
	}
	
	class baseParameter
	{
	public:
		virtual void setParameter(std::string value_arg) = 0;
		virtual std::string getStringValue() = 0;
		std::string name;
	};
	
	baseParameter *parameterPool[numbOfParam];
	
	class boolParameter : public baseParameter
	{
	public:
		boolParameter(std::string p_name, bool *value_arg, bool defaultValue_arg)
		{
			(*this).name = p_name;
			p_parameter = value_arg;
			*p_parameter = defaultValue_arg;
		}
	
		void setParameter(std::string value_arg)
		{
			if(value_arg == "1")
			{
				*p_parameter = true;
				Message::show(MSG_INFO, MSG_CONFIG, (name + ": True"));
			}
			else
			{
				*p_parameter = false;
				Message::show(MSG_INFO, MSG_CONFIG, (name + ": False"));
			}
		}
	
		std::string getStringValue()
		{
			if(*p_parameter == true)
				return "1";
			else
				return "0";
		}
	
		bool *p_parameter;
	};
	class intParameter : public baseParameter
	{
	public:
		intParameter(std::string p_name, int *value_arg, int defaultValue_arg)
		{
			(*this).name = p_name;
			p_parameter = value_arg;
			*p_parameter = defaultValue_arg;
		}
	
		void setParameter(std::string value_arg)
		{
			*p_parameter = std::atoi(value_arg.c_str());
			Message::show(MSG_INFO, MSG_CONFIG, (name + ": " + Message::toString(*p_parameter)));
		}
	
		std::string getStringValue()
		{
			return Message::toString(*p_parameter);
		}
	
		int *p_parameter;
	};
	class floatParameter : public baseParameter
	{
	public:
		floatParameter(std::string p_name, float *value_arg, float defaultValue_arg)
		{
			(*this).name = p_name;
			p_parameter = value_arg;
			*p_parameter = defaultValue_arg;
		}
	
		void setParameter(std::string value_arg)
		{
			*p_parameter = std::atof(value_arg.c_str());
		}
	
		std::string getStringValue()
		{
			return Message::toString(*p_parameter);
		}
	
		float *p_parameter;
	};
	class stringParameter : public baseParameter
	{
	public:
		stringParameter(std::string p_name, std::string *value_arg, std::string defaultValue_arg)
		{
			(*this).name = p_name;
			p_parameter = value_arg;
			*p_parameter = defaultValue_arg;
		}
	
		void setParameter(std::string value_arg)
		{
			*p_parameter = value_arg;
			Message::show(MSG_INFO, MSG_CONFIG, (name + ": " + *p_parameter));
		}
	
		std::string getStringValue()
		{
			return *p_parameter;
		}
	
		std::string *p_parameter;
	};
	class GLenumParameter : public baseParameter
	{
	public:
		GLenumParameter(std::string p_name, GLenum *value_arg, GLenum defaultValue_arg)
		{
			(*this).name = p_name;
			p_parameter = value_arg;
			*p_parameter = defaultValue_arg;
		}
	
		void setParameter(std::string value_arg)
		{
			if(value_arg == "GL_NEAREST")
			{
				*p_parameter = GL_NEAREST;
				Message::show(MSG_INFO, MSG_CONFIG, (name + ": GL_NEAREST"));
			}
			else
				if(value_arg == "GL_LINEAR")
				{
					*p_parameter = GL_LINEAR;
					Message::show(MSG_INFO, MSG_CONFIG, (name + ": GL_LINEAR"));
				}
				else
					if(value_arg == "GL_NEAREST_MIPMAP_NEAREST")
					{
						*p_parameter = GL_NEAREST_MIPMAP_NEAREST;
						Message::show(MSG_INFO, MSG_CONFIG, (name + ": GL_NEAREST_MIPMAP_NEAREST"));
					}
					else
						if(value_arg == "GL_LINEAR_MIPMAP_NEAREST")
						{
							*p_parameter = GL_LINEAR_MIPMAP_NEAREST;
							Message::show(MSG_INFO, MSG_CONFIG, (name + ": GL_LINEAR_MIPMAP_NEAREST"));
						}
						else
							if(value_arg == "GL_NEAREST_MIPMAP_LINEAR")
							{
								*p_parameter = GL_NEAREST_MIPMAP_LINEAR;
								Message::show(MSG_INFO, MSG_CONFIG, (name + ": GL_NEAREST_MIPMAP_LINEAR"));
							}
							else
								if(value_arg == "GL_LINEAR_MIPMAP_LINEAR")
								{
									*p_parameter = GL_LINEAR_MIPMAP_LINEAR;
									Message::show(MSG_INFO, MSG_CONFIG, (name + ": GL_LINEAR_MIPMAP_LINEAR"));
								}
								else
								{
									std::stringstream ss;
									ss << value_arg;
									ss >> std::hex >> *p_parameter;
									Message::show(MSG_INFO, MSG_CONFIG, (name + ": " + value_arg));
								}
		}
	
		std::string getStringValue()
		{
			if(*p_parameter == GL_NEAREST)
			{
				return "GL_NEAREST";
			}
			else
				if(*p_parameter == GL_LINEAR)
				{
					return "GL_LINEAR";
				}
				else
					if(*p_parameter == GL_NEAREST_MIPMAP_NEAREST)
					{
						return "GL_NEAREST_MIPMAP_NEAREST";
					}
					else
						if(*p_parameter == GL_LINEAR_MIPMAP_NEAREST)
						{
							return "GL_LINEAR_MIPMAP_NEAREST";
						}
						else
							if(*p_parameter == GL_NEAREST_MIPMAP_LINEAR)
							{
								return "GL_NEAREST_MIPMAP_LINEAR";
							}
							else
								if(*p_parameter == GL_LINEAR_MIPMAP_LINEAR)
								{
									return "GL_LINEAR_MIPMAP_LINEAR";
								}
								else
								{
									return Message::toString(*p_parameter);
								}
		}
	
		GLenum *p_parameter;
	};
	
	void initValue(std::string p_name, bool *value_arg, bool defaultValue_arg)
	{
		parameterPool[currentIndex] = new boolParameter(p_name, *&value_arg, defaultValue_arg);
		currentIndex++;
	}
	void initValue(std::string p_name, int *value_arg, int defaultValue_arg)
	{
		parameterPool[currentIndex] = new intParameter(p_name, *&value_arg, defaultValue_arg);
		currentIndex++;
	}
	void initValue(std::string p_name, float *value_arg, float defaultValue_arg)
	{
		parameterPool[currentIndex] = new floatParameter(p_name, *&value_arg, defaultValue_arg);
		currentIndex++;
	}
	void initValue(std::string p_name, std::string *value_arg, std::string defaultValue_arg)
	{
		parameterPool[currentIndex] = new stringParameter(p_name, *&value_arg, defaultValue_arg);
		currentIndex++;
	}
	void initValue(std::string p_name, GLenum *value_arg, GLenum defaultValue_arg)
	{
		parameterPool[currentIndex] = new GLenumParameter(p_name, *&value_arg, defaultValue_arg);
		currentIndex++;
	}
	
	void init()
	{
		initValue("window_name", &window::name, "Paradime3D");
		initValue("window_position_x", &window::position_x, 0);
		initValue("window_position_y", &window::position_y, 0);
		initValue("window_size_windowed_x", &window::size_windowed_x, 800);
		initValue("window_size_windowed_y", &window::size_windowed_y, 600);
		initValue("window_size_fullscreen_x", &window::size_fullscreen_x, 1600);
		initValue("window_size_fullscreen_y", &window::size_fullscreen_y, 900);
		initValue("default_display", &window::default_display, 0);
		initValue("window_fullscreen", &window::fullscreen, false);
		initValue("mouse_clipped", &window::mouse_clipped, false);
		initValue("engine_delta_time_divider", &engine::delta_time_divider, 1000);
		initValue("gl_context_major_version", &engine::gl_context_major_version, 3);
		initValue("gl_context_minor_version", &engine::gl_context_minor_version, 3);
		initValue("multisample_buffers", &engine::multisample_buffers, 1);
		initValue("multisample_samples", &engine::multisample_samples, 4);
		initValue("face_culling_mode", &engine::face_culling_mode, 0x0405);
		initValue("depth_test_func", &engine::depth_test_func, 0x0201);
		initValue("multisampling", &engine::multisampling, true);
		initValue("face_culling", &engine::face_culling, true);
		initValue("depth_test", &engine::depth_test, true);
		initValue("vertical_sync", &engine::vertical_sync, true);
		initValue("z_far", &engine::z_far, 2000.0f);
		initValue("z_near", &engine::z_near, 0.1f);
		initValue("stencil_pass_vert_shader", &engine::stencil_pass_vert_shader, "stencilPass.pvert");
		initValue("stencil_pass_frag_shader", &engine::stencil_pass_frag_shader, "stencilPass.pfrag");
		initValue("gl_texture_minification", &texture::gl_texture_magnification, GL_LINEAR_MIPMAP_LINEAR);
		initValue("gl_texture_magnification", &texture::gl_texture_minification, GL_LINEAR);
		initValue("gl_texture_anisotropy", &texture::gl_texture_anisotropy, 0);
		initValue("default_texture", &texture::default_texture, "default.png");
		initValue("textures_path", &path::textures_path, "Materials\\");
		initValue("default_specular_power", &texture::default_specular_power, 32.0f);
		initValue("default_specular_intensity", &texture::default_specular_intensity, 1.0f);
		initValue("models_path", &path::models_path, "Models\\");
		initValue("shader_path", &path::shader_path, "Shaders\\");
		initValue("sounds_path", &path::sounds_path, "Sounds\\");
		initValue("maps_path", &path::maps_path, "Maps\\");
		initValue("objects_path", &path::objects_path, "Objects\\");
		initValue("key_forward", &keys::forward, 26);
		initValue("key_backward", &keys::backward, 22);
		initValue("key_left", &keys::left, 4);
		initValue("key_right", &keys::right, 7);
		initValue("key_jump", &keys::jump, 44);
		initValue("key_fullscreen", &keys::fullscreen, 69);
		initValue("key_vsync", &keys::vsync, 68);
		initValue("key_clip_mouse", &keys::clip_mouse, 67);
		initValue("key_debug_1", &keys::debug_1, 58);
		initValue("key_esc", &keys::escKey, 41);
		initValue("key_back", &keys::backKey, 42);
		initValue("key_arrow_up", &keys::arrow_up, 82);
		initValue("key_arrow_down", &keys::arrow_down, 81);
		initValue("key_arrow_left", &keys::arrow_left, 80);
		initValue("key_arrow_right", &keys::arrow_right, 79);
		initValue("mouse_sensitivity", &keys::mouse_sensitivity, 0.001f);
		initValue("camera_freelook_speed", &game::camera_freelook_speed, 10.0f);
		initValue("FOV", &graphics::FOV, 60.0f);
		initValue("pointlights_max", &graphics::pointlights_max, 20);
		initValue("spotlights_max", &graphics::spotlights_max, 20);
		initValue("dir_light_vert_shader", &renderer::dir_light_vert_shader, "dirLightPass.pvert");
		initValue("dir_light_frag_shader", &renderer::dir_light_frag_shader, "dirLightPass.pfrag");
		initValue("point_light_vert_shader", &renderer::point_light_vert_shader, "pointLightPass.pvert");
		initValue("point_light_frag_shader", &renderer::point_light_frag_shader, "pointLightPass.pfrag");
		initValue("spot_light_vert_shader", &renderer::spot_light_vert_shader, "pointLightPass.pvert");
		initValue("spot_light_frag_shader", &renderer::spot_light_frag_shader, "pointLightPass.pfrag");
		initValue("dir_light_quad", &renderer::dir_light_quad, "quad.obj");
		initValue("point_light_sphere", &renderer::point_light_sphere, "sphere.obj");
		initValue("stencil_pass_vert_shader", &renderer::stencil_pass_vert_shader, "stencilPass.pvert");
		initValue("stencil_pass_frag_shader", &renderer::stencil_pass_frag_shader, "stencilPass.pfrag");
		initValue("dir_light_quad_offset_x", &renderer::dir_light_quad_offset_x, 0.0f);
		initValue("dir_light_quad_offset_y", &renderer::dir_light_quad_offset_y, 0.0f);
		initValue("dir_light_quad_offset_z", &renderer::dir_light_quad_offset_z, -.5f);
		initValue("dir_light_quad_rotation_x", &renderer::dir_light_quad_rotation_x, 180.0f);
		initValue("dir_light_quad_rotation_y", &renderer::dir_light_quad_rotation_y, 0.0f);
		initValue("dir_light_quad_rotation_z", &renderer::dir_light_quad_rotation_z, 0.0f);
		
		try
		{
			load();
		}
		catch(Message::messageCode error)
		{
			error.display();
		}
	}
	void load()
	{
		std::ifstream configFile(configFileName, std::ifstream::in);
		std::string singleWord;
		
		//configFile.open(configFileName);//, std::ios::in);
	
		if(configFile.fail())
		{
			throw Message::messageCode(MSG_FATAL_ERROR, MSG_CONFIG, "Failed to open the configuration file");
		}
	
		while(!configFile.eof())
		{
			configFile >> singleWord;
			for(int i=0; i<numbOfParam; i++)
			{
				if(singleWord == parameterPool[i]->name)
				{
					configFile >> singleWord;
					parameterPool[i]->setParameter(singleWord);
				}
			}
		}
	
		configFile.close();
	}
	void save()
	{
		std::string stringToWrite, singleWord;
		std::fstream configFile;
		bool parameterMatched = false;
		configFile.open(configFileName, std::ios::in);
	
		if(configFile.fail())
		{
			throw "Failed to open config file";
			std::cout << "Failed to open config file" << std::endl;
		}
	
		while(!configFile.eof())
		{
			parameterMatched = false;
			configFile >> singleWord;
			stringToWrite += singleWord;
	
			for(int i=0; i<numbOfParam; i++)
			{
				if(singleWord == parameterPool[i]->name)
				{
					stringToWrite += " ";
					stringToWrite += parameterPool[i]->getStringValue();
					parameterMatched = true;
				}
			}
			
			configFile >> singleWord;
	
			if(!parameterMatched)
			{
				stringToWrite += " ";
				stringToWrite += singleWord;
			}
	
			stringToWrite += "\n";
		}
	
		std::cout << stringToWrite << std::endl;
		
		configFile.close();
		
		std::ofstream configFileWrite;
		configFileWrite.open(configFileName, std::ios::out);
	
		if(configFileWrite.fail())
		{
			throw "Failed to open config file";
			std::cout << "Failed to open config file" << std::endl;
		}
	
		configFileWrite.clear();
		configFileWrite << stringToWrite;
		configFileWrite.close();
	}
	
}