#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "ErrorHandler.h"
#include "Loader_Shader.h"

namespace ShaderLoader
{
	std::vector<VertFragShader*> vertFragShaderPool;
	std::vector<GeomShader*> geomShaderPool;

	GLuint currentShaderProgram;
	
	BaseShader::BaseShader()
	{
		dirty = true;
		shaderProgram = glCreateProgram();
	}
	BaseShader::~BaseShader()
	{
		glDeleteProgram(shaderProgram);
	}
	void BaseShader::bind()
	{
		// Check if the shader is not being used already, if it is, do nothing to minimize overhead
		//if(shaderProgram != currentShaderProgram)
		//{
			glUseProgram(shaderProgram);
			//currentShaderProgram = shaderProgram;
		//}
	}
	void BaseShader::update()
	{
		bind();

		uniforms->updateFrame();
		uniforms->updateModel();
	}
	void BaseShader::updateFrame()
	{
		bind();
		uniforms->updateFrame();
	}
	void BaseShader::updateModel()
	{
		bind();
		uniforms->updateModel();
	}
	void BaseShader::updateMesh()
	{
		bind();
		uniforms->updateModel();
	}
	void BaseShader::updateTextureUniforms()
	{
		bind();
		uniforms->updateTextureUniforms();
	}
	std::string BaseShader::loadFromFile(std::string sourceFileName_arg)
	{
		std::string shaderSource_ret;
		std::ifstream sourceFileStream(sourceFileName_arg, std::ios::in);
		if(sourceFileStream.is_open())
		{
			std::string lineTemp = "";
			while(getline(sourceFileStream, lineTemp))
				shaderSource_ret += "\n" + lineTemp;
			sourceFileStream.close();
			Message::show(MSG_INFO, MSG_SHADER, sourceFileName_arg + ": source code loaded.");
		}
		else
		{
			throw Message::messageCode(MSG_FATAL_ERROR, MSG_SHADER, sourceFileName_arg + ": source code file has failed to load.");
		}
		return shaderSource_ret;
	}
	void BaseShader::compileFromSource(GLuint shaderToCompile_arg, std::string shaderSource_arg, std::string shaderName_arg)
	{
		char const * shaderSourcePointer;
		GLint shaderCompileResult;

		// Get the source code of a shader
		shaderSourcePointer = (shaderSource_arg).c_str();
		glShaderSource(shaderToCompile_arg, 1, &(shaderSourcePointer) , NULL);
		glCompileShader(shaderToCompile_arg);
		
		// Check for shader compilation errors
		glGetShaderiv(shaderToCompile_arg, GL_COMPILE_STATUS, &shaderCompileResult);
		if(!shaderCompileResult)
		{
			int shaderCompileLogLength;
			glGetShaderiv(shaderToCompile_arg, GL_INFO_LOG_LENGTH, &shaderCompileLogLength);

			// Get the actual error message
			std::vector<char> shaderCompileErrorMessage(shaderCompileLogLength);
			glGetShaderInfoLog(shaderToCompile_arg, shaderCompileLogLength, NULL, &shaderCompileErrorMessage[0]);

			// Convert vector of chars to a string
			std::string errorMessageTemp;
			for(int i=0; shaderCompileErrorMessage[i]; i++)
				errorMessageTemp += shaderCompileErrorMessage[i];

			throw Message::messageCode(MSG_FATAL_ERROR, MSG_SHADER, shaderName_arg + ": " + errorMessageTemp);
		}
		else
		{
			Message::show(MSG_INFO, MSG_SHADER, shaderName_arg + ": shader compiled successfully.");
		}
	}
	void BaseShader::attachShader(GLuint shaderToAttach_arg)
	{
		glAttachShader(shaderProgram, shaderToAttach_arg);
	}
	void BaseShader::linkProgram()
	{
		GLint shaderLinkingResult;
		glLinkProgram(shaderProgram);

		// Check for linking errors. If an error has occured, get the error message and throw an exception
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderLinkingResult);
		if(!shaderLinkingResult)
		{
			int shaderLinkLogLength;
			glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &shaderLinkLogLength);
			
			// Get the actual error message
			std::vector<char> shaderLinkErrorMessage(shaderLinkLogLength);
			glGetShaderInfoLog(shaderProgram, shaderLinkLogLength, NULL, &shaderLinkErrorMessage[0]);

			// Convert vector of chars to a string
			std::string ErrorMessageTemp;
			for(int i=0; shaderLinkErrorMessage[i]; i++)
				ErrorMessageTemp += shaderLinkErrorMessage[i];
			
			throw Message::messageCode(MSG_FATAL_ERROR, MSG_SHADER, "Shader linking has failed: " + ErrorMessageTemp);
		}
		else
		{
			Message::show(MSG_INFO, MSG_SHADER, "Shaders linked successfully.");
		}
	}

	VertFragShader::VertFragShader(std::string vertexShaderName_arg, std::string fragmentShaderName_arg)
	{
		vertexShaderName = vertexShaderName_arg;
		fragmentShaderName = fragmentShaderName_arg;
	}
	VertFragShader::~VertFragShader()
	{

	}
	void VertFragShader::init()
	{
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		load(Config::path::shader_path + vertexShaderName, Config::path::shader_path + fragmentShaderName);
		compile();
		link();

		//releaseShaderHandles();

		uniforms = new AdaptiveUniforms(&shaderProgram);
		
		Message::show(MSG_INFO, MSG_SHADER, vertexShaderName + " (Vertex) and " + fragmentShaderName + " (Fragment) handles created.");

		Message::show(MSG_INFO, MSG_SHADER, vertexShaderName + ": Uniform updates per frame: " + Message::toString(uniforms->getNumUpdatesPerFrame()));
		Message::show(MSG_INFO, MSG_SHADER, vertexShaderName + ": Uniform updates per model: " + Message::toString(uniforms->getNumUpdatesPerModel()));
		Message::show(MSG_INFO, MSG_SHADER, vertexShaderName + ": Uniform updates per mesh: " + Message::toString(uniforms->getNumUpdatesPerMesh()));
		Message::show(MSG_INFO, MSG_SHADER, vertexShaderName + ": Texture uniform updates: " + Message::toString(uniforms->getNumTextureUpdates()));
	}
	void VertFragShader::load(std::string vertexShaderName_arg, std::string fragmentShaderName_arg)
	{
		try
		{
			vertexShaderSource = loadFromFile(vertexShaderName_arg);
			fragmentShaderSource = loadFromFile(fragmentShaderName_arg);
		}
		catch(Message::messageCode error)
		{
			error.display();
		}
	}
	void VertFragShader::compile()
	{
		try
		{
			compileFromSource(vertexShader, vertexShaderSource, vertexShaderName);
			compileFromSource(fragmentShader, fragmentShaderSource, fragmentShaderName);
		}
		catch(Message::messageCode error)
		{
			error.display();
		}
	}
	void VertFragShader::link()
	{
		attachShader(vertexShader);
		attachShader(fragmentShader);

		try
		{
			linkProgram();
		}
		catch(Message::messageCode error)
		{
			error.display();
		}
	}
	void VertFragShader::releaseShaderHandles()
	{
		// Delete compiled shaders, after linking them to the shader program
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	bool VertFragShader::compareName(std::string vertexShaderName_arg, std::string fragmentShaderName_arg)
	{
		return (vertexShaderName == vertexShaderName_arg && fragmentShaderName == fragmentShaderName_arg);
	}

	GeomShader::GeomShader(std::string geometryShaderName_arg, std::string vertexShaderName_arg, std::string fragmentShaderName_arg) : VertFragShader(vertexShaderName_arg, fragmentShaderName_arg)
	{
		geometryShaderName = geometryShaderName_arg;
	}
	GeomShader::~GeomShader()
	{

	}
	void GeomShader::init()
	{
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

		load(Config::path::shader_path + geometryShaderName, Config::path::shader_path + vertexShaderName, Config::path::shader_path + fragmentShaderName);
		compile();
		link();

		//releaseShaderHandles();

		uniforms = new AdaptiveUniforms(&shaderProgram);
		
		Message::show(MSG_INFO, MSG_SHADER, vertexShaderName + " (Vertex) and " + fragmentShaderName + " (Fragment) handles created.");

		Message::show(MSG_INFO, MSG_SHADER, vertexShaderName + ": Uniform updates per frame: " + Message::toString(uniforms->getNumUpdatesPerFrame()));
		Message::show(MSG_INFO, MSG_SHADER, vertexShaderName + ": Uniform updates per model: " + Message::toString(uniforms->getNumUpdatesPerModel()));
		Message::show(MSG_INFO, MSG_SHADER, vertexShaderName + ": Uniform updates per mesh: " + Message::toString(uniforms->getNumUpdatesPerMesh()));
		Message::show(MSG_INFO, MSG_SHADER, vertexShaderName + ": Texture uniform updates: " + Message::toString(uniforms->getNumTextureUpdates()));
	}
	void GeomShader::load(std::string geometryShaderName_arg, std::string vertexShaderName_arg, std::string fragmentShaderName_arg)
	{
		try
		{
			geometryShaderSource = loadFromFile(geometryShaderName_arg);
			
			vertexShaderSource = loadFromFile(vertexShaderName_arg);
			fragmentShaderSource = loadFromFile(fragmentShaderName_arg);
		}
		catch(Message::messageCode error)
		{
			error.display();
		}
		
		//VertFragShader::load(vertexShaderName_arg, fragmentShaderName_arg);
	}
	void GeomShader::compile()
	{
		try
		{
			compileFromSource(geometryShader, geometryShaderSource, geometryShaderName);
			
			compileFromSource(vertexShader, vertexShaderSource, vertexShaderName);
			compileFromSource(fragmentShader, fragmentShaderSource, fragmentShaderName);
		}
		catch(Message::messageCode error)
		{
			error.display();
		}

		//VertFragShader::compile();
	}
	void GeomShader::link()
	{
		attachShader(geometryShader);

		attachShader(vertexShader);
		attachShader(fragmentShader);
		linkProgram();

		//VertFragShader::link();
	}
	void GeomShader::releaseShaderHandles()
	{
		VertFragShader::releaseShaderHandles();
		glDeleteShader(geometryShader);
	}
	bool GeomShader::compareName(std::string geometryShaderName_arg, std::string vertexShaderName_arg, std::string fragmentShaderName_arg)
	{
		return (VertFragShader::compareName(vertexShaderName_arg, fragmentShaderName_arg) && geometryShaderName_arg == geometryShaderName);
	}

	VertFragShader *load(std::string vertexShaderName_arg, std::string fragmentShaderName_arg)
	{
		for(std::vector<ShaderLoader::VertFragShader*>::size_type i=0; i < vertFragShaderPool.size(); i++)
		{
			if(vertFragShaderPool[i]->compareName(vertexShaderName_arg, fragmentShaderName_arg))
			{
				Message::show(MSG_INFO, MSG_SHADER, vertexShaderName_arg + " and " + fragmentShaderName_arg + " pointers linked.");
				return vertFragShaderPool[i];
			}
		}

		vertFragShaderPool.push_back(new ShaderLoader::VertFragShader(vertexShaderName_arg, fragmentShaderName_arg));
		vertFragShaderPool[vertFragShaderPool.size() - 1]->init();
		return vertFragShaderPool[vertFragShaderPool.size() - 1];
	}
	GeomShader *load(std::string geometryShaderName_arg, std::string vertexShaderName_arg, std::string fragmentShaderName_arg)
	{
		for(std::vector<ShaderLoader::GeomShader*>::size_type i=0; i < geomShaderPool.size(); i++)
		{
			if(geomShaderPool[i]->compareName(geometryShaderName_arg, vertexShaderName_arg, fragmentShaderName_arg))
			{
				Message::show(MSG_INFO, MSG_SHADER, vertexShaderName_arg + " and " + fragmentShaderName_arg + " pointers linked.");
				return geomShaderPool[i];
			}
		}

		geomShaderPool.push_back(new ShaderLoader::GeomShader(geometryShaderName_arg, vertexShaderName_arg, fragmentShaderName_arg));
		geomShaderPool[geomShaderPool.size() - 1]->init();
		return geomShaderPool[geomShaderPool.size() - 1];
	}

	void init()
	{
		vertFragShaderPool.clear();
		geomShaderPool.clear();
		
		Message::show(MSG_INFO, MSG_SHADER, "has been initialized.");
	}
	void updateFrame()
	{
		for(unsigned int i=0; i < vertFragShaderPool.size(); i++)
		{
			//if(vertFragShaderPool[i]->dirty)
				vertFragShaderPool[i]->updateFrame();
		}
	}
	void forceReload()
	{

	}
}