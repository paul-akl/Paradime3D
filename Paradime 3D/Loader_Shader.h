#ifndef LOADER_SHADER_NAMESPACE
#define LOADER_SHADER_NAMESPACE

#include <string>
#include <vector>

#include <GL/glew.h>

#include "AdaptiveUniforms.h"
#include "Math.h"

namespace ShaderLoader
{
	class Shader
	{

	public:

		Shader(std::string vertexShaderName_arg, std::string fragmentShaderName_arg);
		~Shader();

		GLuint	shaderProgram,
				vertexShader,
				fragmentShader;

		std::string vertexShaderName,
					fragmentShaderName;

		bool	updateRequired;

		void load(std::string vertexShaderName_arg, std::string fragmentShaderName_arg);
		void loadTechnique(std::string techniqueName_arg);
		void update();
		void updateFrame();
		void updateModel();
		void updateMesh();
		void updateTextureUniforms();
		void compile();
		void link();
		void bind();
		void releaseShaderHandles();
		bool compareName(std::string vertexShaderName_arg, std::string fragmentShaderName_arg);

	private:

		std::string loadFromFile(std::string sourceFileName_arg);
		void compileFromSource(GLuint *shaderToCompile_arg, std::string *shaderSource_arg, std::string *shaderName_arg);
		void attachShader(GLuint *shaderToAttach_arg);
		void linkProgram();

		std::string vertexShaderSource,
					fragmentShaderSource;

		AdaptiveUniforms *uniforms;
	};

	Shader *load(std::string vertexShaderName_arg, std::string fragmentShaderName_arg);
	Shader *load(std::string vertexShaderName_arg, std::string fragmentShaderName_arg, bool updateRequired_arg);

	void init();
	void updateFrame();
	void forceReload();
}

#endif