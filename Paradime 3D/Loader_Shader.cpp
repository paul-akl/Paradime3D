#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "ErrorHandler.h"
#include "Loader_Shader.h"

namespace ShaderLoader
{
	std::vector<Shader*> shaderPool;
	GLuint *currentShaderProgram;

	Shader::Shader(std::string vertexShaderName_arg, std::string fragmentShaderName_arg)
	{
		vertexShaderName = vertexShaderName_arg;
		fragmentShaderName = fragmentShaderName_arg;

		shaderProgram = glCreateProgram();
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		updateRequired = true;

		load(Config::path::shader_path + vertexShaderName, Config::path::shader_path + fragmentShaderName);
		compile();
		link();
		//releaseShaderHandles();

		uniforms = new AdaptiveUniforms(&shaderProgram);
		
		Message::show(MSG_INFO, MSG_SHADER, vertexShaderName_arg + " (Vertex) and " + fragmentShaderName_arg + " (Fragment) handles created.");

		Message::show(MSG_INFO, MSG_SHADER, vertexShaderName_arg + ": Uniform updates per frame: " + Message::toString(uniforms->getNumUpdatesPerFrame()));
		Message::show(MSG_INFO, MSG_SHADER, vertexShaderName_arg + ": Uniform updates per model: " + Message::toString(uniforms->getNumUpdatesPerModel()));
		Message::show(MSG_INFO, MSG_SHADER, vertexShaderName_arg + ": Uniform updates per mesh: " + Message::toString(uniforms->getNumUpdatesPerMesh()));
		Message::show(MSG_INFO, MSG_SHADER, vertexShaderName_arg + ": Texture uniform updates: " + Message::toString(uniforms->getNumTextureUpdates()));
	}
	Shader::~Shader()
	{
		releaseShaderHandles();
		glDeleteProgram(shaderProgram);
	}

	void Shader::load(std::string vertexShaderName_arg, std::string fragmentShaderName_arg)
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
	void Shader::loadTechnique(std::string techniqueName_arg)
	{
		//if(shaderTechnique->techniqueName != techniqueName_arg)
		//	shaderTechnique = ShaderTechniques::load(techniqueName_arg);
	}
	void Shader::update()
	{
		bind();

		uniforms->updateFrame();
		uniforms->updateModel();
	}
	void Shader::updateFrame()
	{
		bind();
		uniforms->updateFrame();
	}
	void Shader::updateModel()
	{
		bind();
		uniforms->updateModel();
	}
	void Shader::updateMesh()
	{
		bind();
		uniforms->updateMesh();
	}
	void Shader::updateTextureUniforms()
	{
		bind();
		uniforms->updateTextureUniforms();
	}
	void Shader::compile()
	{
		try
		{
			compileFromSource(&vertexShader, &vertexShaderSource, &vertexShaderName);
			compileFromSource(&fragmentShader, &fragmentShaderSource, &fragmentShaderName);
		}
		catch(Message::messageCode error)
		{
			error.display();
		}
	}
	void Shader::link()
	{
		attachShader(&vertexShader);
		attachShader(&fragmentShader);

		try
		{
			linkProgram();
		}
		catch(Message::messageCode error)
		{
			error.display();
		}
	}
	void Shader::bind()
	{
		// Check if the shader is not being used already, if it is, do nothing to minimize overhead
		if(&shaderProgram != currentShaderProgram)
		{
			glUseProgram(shaderProgram);
			currentShaderProgram = &shaderProgram;
			//Message::show(MSG_INFO, MSG_SHADER, "Shader bound");
		}
	}
	void Shader::releaseShaderHandles()
	{
		// Delete compiled shaders, after linking them to the shader program
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	bool Shader::compareName(std::string vertexShaderName_arg, std::string fragmentShaderName_arg)
	{

		return (vertexShaderName == vertexShaderName_arg && fragmentShaderName == fragmentShaderName_arg);
	}

	std::string Shader::loadFromFile(std::string sourceFileName_arg)
	{
		std::string shaderSource_ret;
		std::ifstream sourceFileStream(sourceFileName_arg, std::ios::in);
		if(sourceFileStream.is_open())
		{
			std::string lineTemp = "";
			//getline(sourceFileStream, lineTemp);	// Read the first line of a shader
			//loadTechnique(lineTemp);				// and load a shader technique 
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
	void Shader::compileFromSource(GLuint *shaderToCompile_arg, std::string *shaderSource_arg, std::string *shaderName_arg)
	{
		char const * shaderSourcePointer;
		GLint shaderCompileResult;

		// Get the source code of a shader
		shaderSourcePointer = (*shaderSource_arg).c_str();
		glShaderSource(*shaderToCompile_arg, 1, &(shaderSourcePointer) , NULL);
		glCompileShader(*shaderToCompile_arg);
		
		// Check for shader compilation errors
		glGetShaderiv(*shaderToCompile_arg, GL_COMPILE_STATUS, &shaderCompileResult);
		if(!shaderCompileResult)
		{
			int shaderCompileLogLength;
			glGetShaderiv(*shaderToCompile_arg, GL_INFO_LOG_LENGTH, &shaderCompileLogLength);

			// Get the actual error message
			std::vector<char> shaderCompileErrorMessage(shaderCompileLogLength);
			glGetShaderInfoLog(*shaderToCompile_arg, shaderCompileLogLength, NULL, &shaderCompileErrorMessage[0]);

			// Convert vector of chars to a string
			std::string errorMessageTemp;
			for(int i=0; shaderCompileErrorMessage[i]; i++)
				errorMessageTemp += shaderCompileErrorMessage[i];

			throw Message::messageCode(MSG_FATAL_ERROR, MSG_SHADER, *shaderName_arg + ": " + errorMessageTemp);
		}
		else
		{
			Message::show(MSG_INFO, MSG_SHADER, *shaderName_arg + ": shader compiled successfully.");
		}
	}
	void Shader::attachShader(GLuint *shaderToAttach_arg)
	{
		glAttachShader(shaderProgram, *shaderToAttach_arg);
	}
	void Shader::linkProgram()
	{
		GLint shaderLinkingResult;

		glLinkProgram(shaderProgram);

		// Check for linking errors. If an error has occured, get the error message and throw an exception
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderLinkingResult);
		if(!shaderLinkingResult)
		{
			int shaderLinkLogLength;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &shaderLinkLogLength);
			
			// Get the actual error message
			std::vector<char> shaderLinkErrorMessage(shaderLinkLogLength);
			glGetShaderInfoLog(vertexShader, shaderLinkLogLength, NULL, &shaderLinkErrorMessage[0]);

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
	
	Shader *load(std::string vertexShaderName_arg, std::string fragmentShaderName_arg)
	{
		for(int i=0; i < shaderPool.size(); i++)
		{
			if(shaderPool[i]->compareName(vertexShaderName_arg, fragmentShaderName_arg))
			{
				Message::show(MSG_INFO, MSG_SHADER, vertexShaderName_arg + " and " + fragmentShaderName_arg + " pointers linked.");
				return shaderPool[i];
			}
		}

		shaderPool.push_back(new ShaderLoader::Shader(vertexShaderName_arg, fragmentShaderName_arg));
		return shaderPool[shaderPool.size() - 1];
	}
	Shader *load(std::string vertexShaderName_arg, std::string fragmentShaderName_arg, bool updateRequired_arg)
	{
		for(int i=0; i < shaderPool.size(); i++)
			if(shaderPool[i]->compareName(vertexShaderName_arg, fragmentShaderName_arg))
			{
				Message::show(MSG_INFO, MSG_SHADER, vertexShaderName_arg + " and " + fragmentShaderName_arg + " pointers linked.");
				shaderPool[i]->updateRequired = updateRequired_arg;
				return shaderPool[i];
			}

		shaderPool.push_back(new Shader(vertexShaderName_arg, fragmentShaderName_arg));
		shaderPool[shaderPool.size() - 1]->updateRequired = updateRequired_arg;
		return shaderPool[shaderPool.size() - 1];
	}

	void init()
	{
		shaderPool.clear();
		
		Message::show(MSG_INFO, MSG_SHADER, "has been initialized.");
	}
	void updateFrame()
	{
		for(unsigned int i=0; i < shaderPool.size(); i++)
		{
			if(shaderPool[i]->updateRequired)
				shaderPool[i]->updateFrame();
		}
	}
	void forceReload()
	{

	}
}