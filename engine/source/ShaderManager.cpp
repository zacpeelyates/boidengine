// File: ShaderManager.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/02/16
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Function implementations for singleton class that manages loading, storing and deleting shader references

#include "ShaderManager.h"

#include <iostream>

#include "Utilities.h"
#include "glad/glad.h"


//singleton pattern implementation
ShaderManager* ShaderManager::m_instance = nullptr;

auto ShaderManager::CreateInstance() -> ShaderManager*
{
	if (m_instance == nullptr)
	{
		m_instance = new ShaderManager();
	}
	return m_instance;
}

auto ShaderManager::GetInstance() -> ShaderManager*
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
auto ShaderManager::CreateProgram(const int& a_vertexShader, const int& a_fragmentShader, const int& a_computeShader) ->
unsigned int
{
	//public static function to call internal program creation
	ShaderManager* instance = GetInstance();
	return ShaderManager::CreateProgramInternal(a_vertexShader, a_fragmentShader, a_computeShader);
}

auto ShaderManager::CreateProgramInternal(const int& a_vertexShader, const int& a_fragmentShader,
                                          const int& a_computeShader)
-> unsigned int
{
	int result = GL_FALSE;

	//create shader program and attach shaders 
	const unsigned int handle = glCreateProgram();
	glAttachShader(handle, a_vertexShader);
	glAttachShader(handle, a_fragmentShader);
	if (a_computeShader != -1)
	{
		glAttachShader(handle, a_computeShader);
	}
	//link shaders to program 
	glLinkProgram(handle);
	//check if program was created
	glGetProgramiv(handle, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		//if program fails to create, print error log
		int infoLogLength = 0;
		glGetProgramiv(handle,GL_INFO_LOG_LENGTH, &infoLogLength);
		auto* const infoLog = new char[infoLogLength];
		glGetProgramInfoLog(handle, infoLogLength, nullptr, infoLog);
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
	ShaderManager* instance = GetInstance();
	instance->DeleteShaderInternal(a_uiShaderID);
}

void ShaderManager::DeleteShaderInternal(unsigned int a_uiShaderID)
{
	//delete passed in shader
	for (auto iter = m_shaders.begin(); iter != m_shaders.end(); ++iter)
	{
		if (iter->second = a_uiShaderID != 0u)
		{
			glDeleteShader(iter->second);
			break;
		}
	}
}

void ShaderManager::DeleteProgram(unsigned int a_program)
{
	//public function to call program deletion while insuring there is a valid instance 
	ShaderManager* instance = GetInstance();
	instance->DeleteProgramInternal(a_program);
}

void ShaderManager::DeleteProgramInternal(unsigned int a_program)
{
	//delete passed in program 
	for (auto iter = m_programs.begin(); iter != m_programs.end(); ++iter)
	{
		if (iter->second = a_program != 0u)
		{
			glDeleteProgram(iter->second);
			break;
		}
	}
}

ShaderManager::ShaderManager() = default;

ShaderManager::~ShaderManager()
{
	//delete shaders
	for (auto iter = m_shaders.begin(); iter != m_shaders.end(); ++iter)
	{
		glDeleteShader(iter->second);
	}
	//delete programs
	for (auto iter = m_programs.begin(); iter != m_programs.end(); ++iter)
	{
		glDeleteProgram(iter->second);
	}
}


auto ShaderManager::LoadShader(const char* a_strShaderFile, unsigned int a_eShaderType) -> GLuint
{
	//public function to call shader load while insuring there is a valid instance
	ShaderManager* instance = GetInstance();
	return instance->LoadShaderInternal(a_strShaderFile, a_eShaderType);
}

auto ShaderManager::LoadShaderInternal(const char* a_filePath, unsigned int a_type) -> unsigned int
{
	//get shader from file
	const char* source = Utilities::FileToBuffer(a_filePath);
	if (source == nullptr)
	{
		return -1;
	}
	GLuint shader = glCreateShader(a_type);
	//set source buffer & compile 
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	//delete source as buffer has been allocated
	delete[] source;

	//test shader for errors
	int result = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		//if shader failed to load print out the error info
		const int infoLogLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
		auto* const infoLog = new char[infoLogLength];
		glGetShaderInfoLog(shader, infoLogLength, nullptr, infoLog);
		std::cout << "Compile Failed: " << a_filePath << std::endl;
		std::cout << infoLog << std::endl;
		delete[] infoLog;
		return 0;
	}
	//add successfully loaded shader to member map 
	m_shaders.emplace(a_filePath, shader);
	return shader;
}
