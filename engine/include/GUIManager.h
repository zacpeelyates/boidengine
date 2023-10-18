// File: GUIManager.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/05/01
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Declarations for IMGUI-Based  GUI controller

#ifndef GUIMANAGER_H
#define GUIMANAGER_H
#include <string>
#include <vector>
#include <imgui_impl_glfw.h>
#include "ImGuizmo.h" //https://github.com/CedricGuillemet/ImGuizmo -- extentions for ImGUI

#define IMGUI_STATIC_INFO_FLAGS ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav

class GUIManager 
{

public:
	//singleton 
	static GUIManager* GetInstance();
	static GUIManager* CreateInstance();
	static void DestroyInstance();
	//setup
	static bool Init(GLFWwindow* window);
	//upkeep
	static void NewFrame();
	static void Render();
	//IMGUI ELEMENTS
	void ShowFrameData();
	void SetupNextWindow();
	bool ShowFileLoader(std::string& input);
	bool ShowLoadedFileList(std::vector<std::string> loadedFiles, std::string& selectedFile);
	bool ShowColorEditor(float* firstElement, bool alpha = false, const char* title = "ColorEditor");
	bool ShowSlider(float* valueToEdit, float a_min, float a_max);
	bool ShowSliderInt(int* valueToEdit, int a_min, int a_max);
	bool ShowMatrixEditWindow(float matrixToEdit[16], const float viewMatrix[16], const float projectionMatrix[16]);
	bool ShowMatrixEditGizmo(float matrixToEdit[16], const float viewMatrix[16], const float projectionMatrix[16]);
	bool ShowViewEditGizmo(float* viewMatrix, float a_length);
	bool ShowVectorEditor(float* valueToEdit);

	
	template<typename ...T>
	static bool Execute(bool(GUIManager::* a_func)(T...), T... a_args, const char* a_title = "##InvokedWindow")
	{
		//wrapper for imgui calls because i hate writing the begin and end stuff all the time and there's some other management this class wants to do anyway
		bool result = false;
		GUIManager* instance = GetInstance();
		instance->SetupNextWindow();
		if (ImGui::Begin(a_title, &instance->m_bShow, IMGUI_STATIC_INFO_FLAGS)) result = std::invoke(a_func, instance, a_args...);
		instance->m_yOffset += PADDING + ImGui::GetWindowHeight();
		ImGui::End();
		return result;
	}

	
	static void Execute(void(GUIManager::* a_func)(),const char* a_title = "##InvokedWindow")
	{
		//version for void because if i make this more generic i will die (other version does not like methods with no args and im not sure what to do about that atm)
		GUIManager* instance = GetInstance();
		instance->SetupNextWindow();
		if (ImGui::Begin(a_title, &instance->m_bShow, IMGUI_STATIC_INFO_FLAGS)) std::invoke(a_func, instance);
		instance->m_yOffset += PADDING + ImGui::GetWindowHeight();
		ImGui::End();
	}
	

private:
	GUIManager();
	~GUIManager();
	static GUIManager* m_instance;
	static const int PADDING;
	int m_yOffset{0};
	int m_xOffset{0};
	int m_columns{4};
	int m_corner{0};
	bool m_bShow{false};
	std::string m_inputBuffer;
	
	static ImGuizmo::OPERATION m_operation;
	static ImGuizmo::MODE m_mode;
};


#endif // GUIMANAGER_H

