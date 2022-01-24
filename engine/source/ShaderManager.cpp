#include "ShaderManager.h"
#include "Utilities.h"
#include "glad/glad.h"
#include <sstream>
#include <fstream>
#include <iostream>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	ShaderManager.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 21/01/22
// Last Edited:  21/01/22
// Brief: Function implementations for singleton class that manages loading, storing and deleting shader references
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//singleton pattern implementation
ShaderManager* ShaderManager::m_instance = nullptr;

ShaderManager* ShaderManager::CreateInstance()
{
	if (m_instance == nullptr) m_instance = new ShaderManager();
	return m_instance;
}

ShaderManager* ShaderManager::GetInstance() 
{
	return m_instance == nullptr ? CreateInstance() : m_instance;
}

void ShaderManager::DestroyInstance()
{
	if (m_instance != nullptr) 
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

//functions
unsigned int ShaderManager::CreateProgram(const int& a_vertexShader, const int& a_fragmentShader) 
{
	//public static function to call internal program creation
	ShaderManager* instance = ShaderManager::GetInstance();
	return instance->CreateProgramInternal(a_vertexShader, a_fragmentShader);
}

unsigned int ShaderManager::CreateProgramInternal(const int& a_vertexShader, const int& a_fragmentShader)
{
	int result = GL_FALSE;

	//create shader program and attach shaders 
	unsigned int handle = glCreateProgram();
	glAttachShader(handle, a_vertexShader);
	glAttachShader(handle, a_fragmentShader);
	//link shaders to program 
	glLinkProgram(handle);
	//check if program was created
	glGetProgramiv(handle, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) 
	{
		//if program fails to create, print error log
		int infoLogLength = 0;
		glGetProgramiv(handle,GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(handle, infoLogLength, 0, infoLog);
		std::cout << "Shader Linker Error" << std::endl;
		std::cout << infoLog << std::endl;

		delete[] infoLog;
		return 0; //null program return 
	}
	return handle;

}


void ShaderManager::DeleteShader(unsigned int a_uiShaderID)
{
	//public function to call shader deletion while insuring there is a valid instance 
	ShaderManager* instance = ShaderManager::GetInstance();
	instance->DeleteShaderInternal(a_uiShaderID);
}

void ShaderManager::DeleteShaderInternal(unsigned int a_uiShaderID)
{
	//delete passed in shader
	for (std::map<std::string, unsigned int>::iterator iter = m_shaders.begin(); iter != m_shaders.end(); ++iter)
	{
		if (iter->second = a_uiShaderID) {
			glDeleteShader(iter->second);
			break;
		}
	}
}

void ShaderManager::DeleteProgram(unsigned int a_program) 
{
	//public function to call program deletion while insuring there is a valid instance 
	ShaderManager* instance = ShaderManager::GetInstance();
	instance->DeleteProgramInternal(a_program);
}

void ShaderManager::DeleteProgramInternal(unsigned int a_program) 
{
	//delete passed in program 
	for (std::map<std::string, unsigned int>::iterator iter = m_programs.begin(); iter != m_programs.end(); ++iter)
	{
		if (iter->second = a_program) {
			glDeleteProgram(iter->second);
			break;
		}
	}
}

ShaderManager::ShaderManager() {}

ShaderManager::~ShaderManager() 
{
	//delete shaders
	for (std::map<std::string,unsigned int>::iterator iter = m_shaders.begin(); iter != m_shaders.end(); ++iter) 
	{
		glDeleteShader(iter->second);
	}
	//delete programs
	for (std::map<std::string, unsigned int>::iterator iter = m_programs.begin(); iter != m_programs.end(); ++iter)
	{
		glDeleteProgram(iter->second);
	}
}


GLuint ShaderManager::LoadShader(const char* a_strShaderFile, unsigned int a_eShaderType)
{
	//public function to call shader load while insuring there is a valid instance
	ShaderManager* instance = ShaderManager::GetInstance();
	return instance->LoadShaderInternal(a_strShaderFile, a_eShaderType);

}

unsigned int ShaderManager::LoadShaderInternal(const char* a_filePath,unsigned int a_type)
{
	//get shader from file
	char* source = Utilities::FileToBuffer(a_filePath);
	if (source == nullptr) return -1;
	GLuint shader = glCreateShader(a_type);
	//set source buffer & compile 
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	//delete source as buffer has been allocated
	delete[] source;

	//test shader for errors
	int result = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE)
	{
		//if shader failed to load print out the error info
		int infoLogLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(shader, infoLogLength, 0, infoLog);
		std::cout << "Compile Failed: " << a_filePath << std::endl;
		std::cout << infoLog << std::endl;
		delete[] infoLog;
		return 0;
	}
	else 
	{
		//add successfully loaded shader to member map 
		m_shaders.emplace(a_filePath, shader);
	}
	return shader;

}