#ifndef LOADER_SHADER_NAMESPACE
#define LOADER_SHADER_NAMESPACE

#include <string>
#include <vector>

#include <GL/glew.h>

#include "AdaptiveUniforms.h"
#include "Math.h"

namespace ShaderLoader
{
	class BaseShader
	{
	public:
		
		BaseShader();
		~BaseShader();

		virtual void init() = 0;
		virtual void link() = 0;
		virtual void compile() = 0;
		virtual void releaseShaderHandles() = 0;
		
		void bind();
		void update();
		void updateFrame();
		void updateModel();
		void updateMesh();
		void updateTextureUniforms();

		GLuint	shaderProgram;
		bool	dirty;

	protected:
		
		std::string loadFromFile(std::string sourceFileName_arg);
		void compileFromSource(GLuint shaderToCompile_arg, std::string shaderSource_arg, std::string shaderName_arg);
		void attachShader(GLuint shaderToAttach_arg);
		void linkProgram();

		AdaptiveUniforms *uniforms;
	};
	class VertFragShader : public BaseShader
	{
	public:

		VertFragShader(std::string vertexShaderName_arg, std::string fragmentShaderName_arg);
		~VertFragShader();
		
		void init();
		void load(std::string vertexShaderName_arg, std::string fragmentShaderName_arg);
		void compile();
		void link();
		void releaseShaderHandles();
		bool compareName(std::string vertexShaderName_arg, std::string fragmentShaderName_arg);
		
		GLuint		vertexShader,
					fragmentShader;
		std::string vertexShaderName,
					fragmentShaderName;

	protected:
		
		std::string vertexShaderSource,
					fragmentShaderSource;
	};
	class GeomShader : public VertFragShader
	{
	public:

		GeomShader(std::string geometryShaderName_arg, std::string vertexShaderName_arg, std::string fragmentShaderName_arg);
		~GeomShader();
		
		void init();
		void load(std::string geometryShaderName_arg, std::string vertexShaderName_arg, std::string fragmentShaderName_arg);
		void compile();
		void link();
		void releaseShaderHandles();
		bool compareName(std::string geometryShaderName_arg, std::string vertexShaderName_arg, std::string fragmentShaderName_arg);

		GLuint		geometryShader;
		std::string geometryShaderName;
		
	private:
		
		std::string geometryShaderSource;

	};

	VertFragShader *load(std::string vertexShaderName_arg, std::string fragmentShaderName_arg);
	GeomShader *load(std::string geometryShaderName_arg, std::string vertexShaderName_arg, std::string fragmentShaderName_arg);

	void init();
	void updateFrame();
	void forceReload();
}

#endif