#pragma once
#include <iostream>
#include<string>
#include<fstream>
#include<sstream>
#include "Renderer.h"

struct ShaderStruct 
{
	std::string vertexShader;
	std::string fragmentShader;

	ShaderStruct(std::string vertexShader, std::string fragmentShader) 
	{
		this->vertexShader = vertexShader;
		this->fragmentShader = fragmentShader;

	}
};


class Shader
{

private:
	unsigned int m_RendererID{};
	std::string filePath;
public:
	Shader(const std::string& filePath)
	{
		std::cout << "hello world this is from shader.h file path " <<filePath<< std::endl;
		this->filePath = filePath;
		ShaderStruct source = this->parseShaderFile();
		this->createShader(source.vertexShader , source.fragmentShader);

		



	}
	~Shader() 
	{
		GLCall(glDeleteProgram(this->m_RendererID));
	}

	void bind() 
	{
		GLCall(glUseProgram(this->m_RendererID));
	}
	void unBind() 
	{
		GLCall(glUseProgram(0));
	}

	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
	{
		GLCall(glUniform4f(this->getUniformLocation(name) , v0 , v1 , v2 , v3));

	}
private:
	ShaderStruct parseShaderFile() 
	{
		std::ifstream stream(this->filePath);
		char type = 0;
		std::string line;
		std::stringstream ss[2];

		while (getline(stream, line)) 
		{
			if (line.find("shader") != std::string::npos) 
			{
				if (line.find("fragment") != std::string::npos) 
				{
					type = 1;
				}
			
			}
			else 
			{
				*(ss + int(type)) << line << "\n";

			}

		}
		std::cout << ss[0].str() << std::endl;

		return ShaderStruct((*(ss)).str(), (*(ss + 1)).str());

			
	}
	unsigned int compileShader(unsigned int type, const std::string& source) 
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str(); // for this code to work properly source must exist in memory or else this pointer will point to rubbish 
		GLCall(glShaderSource(id, 1, &src, nullptr));
		GLCall(glCompileShader(id));
		return id;
	}
	void createShader(const std::string& vertexShader, const std::string& fragmentShader) 
	{
		GLCall(this->m_RendererID = glCreateProgram());
		unsigned int vs = this->compileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = this->compileShader(GL_FRAGMENT_SHADER, fragmentShader);

		GLCall(glAttachShader(this->m_RendererID, vs));
		GLCall(glAttachShader(this->m_RendererID, fs));
		GLCall(glLinkProgram(this->m_RendererID));
		GLCall(glValidateProgram(this->m_RendererID));


		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));

	}
	unsigned int getUniformLocation(const std::string& name) 
	{
		GLCall(int location = glGetUniformLocation(this->m_RendererID , name.c_str()));
		if (location == -1) 
		{
			// do somehandling shit
		}
		return location;

	}

	


};
