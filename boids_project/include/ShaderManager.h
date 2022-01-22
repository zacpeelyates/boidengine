
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	ShaderManager.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 21/01/22
// Last Edited:  21/01/22
// Brief: Function definitions for singleton class that manages loading, storing and deleting shader references
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <string>
#include <map>
class ShaderManager {

public:

	static unsigned int LoadShader(const char* ac_strShaderFile, unsigned int a_uiShaderType);
	static unsigned int CreateProgram(const int& a_irVertexShader, const int& a_irFragmentShader);
	static void DeleteShader(unsigned int a_uiShaderID);
	static void DeleteProgram(unsigned int a_uiProgramID);

	static ShaderManager* CreateInstance();
	static ShaderManager* GetInstance();
	static void DestroyInstance();

private:
	ShaderManager();
	~ShaderManager();

	static ShaderManager* m_instance;

	std::map<std::string, unsigned int> m_shaders;
	std::map<std::string, unsigned int> m_programs;

	unsigned int LoadShaderInternal(const char* a_strShaderFile, unsigned int a_eShaderType);
	unsigned int CreateProgramInternal(const int& a_irVertexShader, const int& a_irFragmentShader);
	void DeleteShaderInternal(unsigned int a_uiShaderID);
	void DeleteProgramInternal(unsigned int a_uiProgramID);
	
};


#endif // SHADERMANAGER_H
