
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	GUIManager.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 03/01/22
// Last Edited:  07/01/22
// Brief: function definitions for IMGUI/IMGUIZMO wrapper class. Handles displaying UI elements and ensuring they don't overlap
// allows for needed UI elements to simply be called in one line instead of filling everything with IMGUI boilerplate code.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef GUIMANAGER_H
#define GUIMANAGER_H
#include <string>
#include <vector>
#include <imgui_impl_glfw.h>
#include "ImGuizmo.h" //https://github.com/CedricGuillemet/ImGuizmo -- extentions for ImGUI


class GUIManager 
{

public:
	static GUIManager* GetInstance();
	static GUIManager* CreateInstance();
	static void DestroyInstance();
	bool Init(GLFWwindow* window);
	void NewFrame();
	void Render();
	void ShowFrameData();
	void SetupNextWindow();
	bool ShowFileLoader(std::string& input);
	bool ShowLoadedFileList(std::vector<std::string> loadedFiles, std::string& selectedFile);
	bool ShowColorEditor(float* firstElement, std::string title = "ColorEditor", bool alpha = false);

	bool ShowSlider(float* valueToEdit, float a_min, float a_max, std::string title);

	bool ShowMatrixEditWindow(float matrixToEdit[16], const float viewMatrix[16], const float projectionMatrix[16]);

	bool ShowMatrixEditGizmo(float matrixToEdit[16], const float viewMatrix[16], const float projectionMatrix[16]);

	void ShowViewEditGizmo(float* viewMatrix, float a_length);

	
	

private:
	GUIManager();
	~GUIManager();
	static GUIManager* m_instance;
	static const int PADDING;
	int m_yOffset;
	int m_corner;
	bool m_bShow;

	std::string m_inputBuffer;
	
	static ImGuizmo::OPERATION m_operation;
	static ImGuizmo::MODE m_mode;

};

#define IMGUI_STATIC_INFO_FLAGS ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav
#endif // GUIMANAGER_H
