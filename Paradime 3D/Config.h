#ifndef CONFIG_CLASS
#define CONFIG_CLASS

//#include <fstream>
//#include <iostream>
//#include <sstream>
#include <string>
#include <GL/glew.h>

namespace Config
{
	namespace window
	{
		extern std::string name;
		extern int position_x;
		extern int position_y;
		extern int size_windowed_x;
		extern int size_windowed_y;
		extern int size_fullscreen_x;
		extern int size_fullscreen_y;
		extern int default_display;
		extern bool fullscreen;
		extern bool mouse_clipped;
	}
	namespace engine
	{
		extern int delta_time_divider;
		extern int gl_context_major_version;
		extern int gl_context_minor_version;
		extern int multisample_buffers;
		extern int multisample_samples;
		extern int face_culling_mode;
		extern int depth_test_func;
		extern bool multisampling;
		extern bool depth_test;
		extern bool face_culling;
		extern bool vertical_sync;
		extern float z_far;
		extern float z_near;
		extern std::string stencil_pass_vert_shader;
		extern std::string stencil_pass_frag_shader;
	}
	namespace texture
	{
		extern std::string default_texture;
		extern GLenum gl_texture_minification;
		extern GLenum gl_texture_magnification;
		extern bool generate_mipmaps;
		extern int number_of_mipmaps;
		extern int gl_texture_anisotropy;
		extern float default_specular_power;
		extern float default_specular_intensity;
	}
	namespace sound
	{
		
	}
	namespace path
	{
		extern std::string textures_path;
		extern std::string models_path;
		extern std::string shader_path;
		extern std::string sounds_path;
		extern std::string maps_path;
		extern std::string objects_path;
	}
	namespace keys
	{
		extern int forward;
		extern int backward;
		extern int left;
		extern int right;
		extern int jump;
		extern int fullscreen;
		extern int vsync;
		extern int clip_mouse;
		extern int debug_1;
		extern int debug_2;
		extern int escKey;
		extern int backKey;
		extern int arrow_up;
		extern int arrow_down;
		extern int arrow_left;
		extern int arrow_right;
		extern float mouse_sensitivity;
	}
	namespace game
	{
		extern float camera_freelook_speed;
	}
	namespace shader
	{
		extern std::string positionVector;
		extern std::string worldViewProjection;
	}
	namespace graphics
	{
		extern float FOV;
		extern float fog_density;
		extern float fog_color_x;
		extern float fog_color_y;
		extern float fog_color_z;
		extern int pointlights_max;
		extern int spotlights_max;
		extern int dir_shadow_res_x;
		extern int dir_shadow_res_y;
	}
	namespace renderer
	{
		extern std::string dir_light_vert_shader;
		extern std::string dir_light_frag_shader;
		extern std::string point_light_vert_shader;
		extern std::string point_light_frag_shader;
		extern std::string spot_light_vert_shader;
		extern std::string spot_light_frag_shader;
		extern std::string dir_light_quad;
		extern std::string point_light_sphere;
		extern std::string stencil_pass_vert_shader;
		extern std::string stencil_pass_frag_shader;
		extern std::string dir_shadowmap_vert_shader;
		extern std::string dir_shadowmap_frag_shader;
		extern std::string dir_basic_shadowmap_vert_shader;
		extern std::string dir_basic_shadowmap_frag_shader;
		extern float dir_light_quad_offset_x;
		extern float dir_light_quad_offset_y;
		extern float dir_light_quad_offset_z;
		extern float dir_light_quad_rotation_x;
		extern float dir_light_quad_rotation_y;
		extern float dir_light_quad_rotation_z;
	}

	extern bool running, resizeWindow, drawDebugBuffers;
	extern int currentScreenWidth, currentScreenHeight;
	extern std::string configFileName;

	void initValue(std::string p_name, bool *value_arg, bool defaultValue_arg);
	void initValue(std::string p_name, std::string *value_arg, std::string defaultValue_arg);
	void initValue(std::string p_name, int *value_arg, int defaultValue_arg);
	void initValue(std::string p_name, float *value_arg, float defaultValue_arg);
	void initValue(std::string p_name, GLenum *value_arg, GLenum defaultValue_arg);

	void init();
	void load();
	void save();
}
#endif