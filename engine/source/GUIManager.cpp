#include "GUIManager.h"
#include <imgui_stdlib.h>
#include <imgui_impl_opengl3.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	GUIManager.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 03/01/22
// Last Edited:  07/01/22
// Brief: Singleton class implementation handling IMGUI and IMGUIZMO library usage. Draw UI to screen each frame.
// May edit some values passed in if called UI element directly modifies values. Most UI elements return a bool
// determining whether or not they were used / interacted with this frame, which can be used by the calling class
// (see RenderWindow.cpp)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const int GUIManager::PADDING = 10;
GUIManager* GUIManager::m_instance = nullptr;
//default values for imguizmo operation and mode
ImGuizmo::OPERATION GUIManager::m_operation = ImGuizmo::ROTATE;
//world mode always used, no parenting structure for local transforms currently
ImGuizmo::MODE GUIManager::m_mode = ImGuizmo::WORLD;

//singleton pattern implementation 
GUIManager* GUIManager::GetInstance()
{
	return m_instance == nullptr ? CreateInstance() : m_instance;
}

GUIManager* GUIManager::CreateInstance()
{
	if (m_instance == nullptr) m_instance = new GUIManager();
	return m_instance;
}

void GUIManager::DestroyInstance()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

GUIManager::GUIManager()
{
	//default constructor, sets default values
	m_yOffset = 0;
	m_corner = 0;
	m_bShow = false;

	//setupimgui
	ImGuizmo::Enable(true);
	ImGuizmo::SetImGuiContext(ImGui::GetCurrentContext());
}

GUIManager::~GUIManager()
{
	//Imgui cleanup
	ImGuizmo::Enable(false);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


bool GUIManager::Init(GLFWwindow* window)
{
	//imgui setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	//if setup failed return false 
	if(!ImGui_ImplGlfw_InitForOpenGL(window, true)) return false;
	if(!ImGui_ImplOpenGL3_Init("#version 400")) return false;
	return true;
}

void GUIManager::NewFrame()
{
	//new IMGUI frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	//new ImGuizmo frame
	ImGuizmo::BeginFrame();
	GetInstance()->m_yOffset = PADDING;
}

void GUIManager::Render()
{
	//IMGUI render
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUIManager::ShowFrameData()
{	
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("FPS: %.1f (Frame Delay: %.3f ms)", io.Framerate, 1000 / io.Framerate);
	if (ImGui::IsMousePosValid())
	{
		ImGui::Text("MousePos: %1.f,%1.f", io.MousePos.x, io.MousePos.y);
	}
	else
	{
		ImGui::Text("MousePos: <offscreen>");
	}
}

void GUIManager::SetupNextWindow()
{
	//ensures next window to display doesn't overlap with other windows
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 windowPos = ImVec2((m_corner & 1) ? io.DisplaySize.x - PADDING : PADDING, (m_corner & 2) ? io.DisplaySize.y - m_yOffset : m_yOffset);
	ImVec2 windowPivot = ImVec2((m_corner & 1) ? 1.0f : 0.0f, (m_corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, windowPivot);
	ImGui::SetNextWindowBgAlpha(0.4f);
}



bool GUIManager::ShowFileLoader(std::string& input)
{
	//show file loading GUI element
	ImGui::InputText("Filename", &m_inputBuffer);
	if (!m_inputBuffer.empty())
	{
		input = m_inputBuffer;
	}
	return ImGui::Button("Load");
}

bool GUIManager::ShowLoadedFileList(std::vector<std::string> loadedFiles, std::string& selectedFile)
{
	for (int i = 0; i < loadedFiles.size(); ++i)
	{
		bool selected = false;
		if (ImGui::Selectable(loadedFiles[i].c_str(), &selected, ImGuiSelectableFlags_DontClosePopups, ImVec2(200, 15)))
		{
			if (selected)
			{
				selectedFile = loadedFiles[i];
				return true;
			}
		}
	}
	return false;

}

bool GUIManager::ShowColorEditor(float* firstElement, const bool alpha, const char* title)
{
	return alpha ? ImGui::ColorEdit4(title, firstElement) : ImGui::ColorEdit3(title, firstElement);
}

bool GUIManager::ShowSlider(float* valueToEdit, const float a_min, const float a_max)
{
	return ImGui::SliderFloat("##defaultSlider", valueToEdit, a_min, a_max);
}

bool GUIManager::ShowMatrixEditWindow(float matrixToEdit[16],const float viewMatrix[16],const float projectionMatrix[16])
{	
	bool b = false;
	//store matrix in easier to edit arrays
	float translation[3];
	float rotation[3];
	float scale[3];
	ImGuizmo::DecomposeMatrixToComponents(matrixToEdit,translation,rotation,scale);

	ImGui::Text("    X         Y         Z    "); //probably the worst possible way to do this but oh well

	//show grid of float to edit arrays
	if (ImGui::InputFloat3("Translation", translation,"%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
	{
		b = true;
		m_operation = ImGuizmo::TRANSLATE;
			
	}
	if (ImGui::InputFloat3("Rotation", rotation, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
	{
		b = true;
		m_operation = ImGuizmo::ROTATE;
	}
	if (ImGui::InputFloat3("Scale", scale, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
	{
		b = true;
		m_operation = ImGuizmo::SCALE;
	}
	//if anything was edited, manipulate the matrix
	if (b)
	{
		ImGuizmo::RecomposeMatrixFromComponents(translation, rotation, scale, matrixToEdit);
		ImGuizmo::Manipulate(viewMatrix, projectionMatrix, m_operation, m_mode, matrixToEdit);
	}
	return b;
}

bool GUIManager::ShowMatrixEditGizmo(float matrixToEdit[16], const float viewMatrix[16], const float projectionMatrix[16])
{
	//click/drag matrix manipulation
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0,0,io.DisplaySize.x,io.DisplaySize.y);
	//toggle edit modes
	if (ImGui::RadioButton("Translate", m_operation == ImGuizmo::TRANSLATE))
	{
		m_operation = ImGuizmo::TRANSLATE;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", m_operation == ImGuizmo::ROTATE))
	{
		m_operation = ImGuizmo::ROTATE;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", m_operation == ImGuizmo::SCALE))
		{
		m_operation = ImGuizmo::SCALE;
	}
	//display and return manipulation gizmo
	return ImGuizmo::Manipulate(viewMatrix, projectionMatrix, m_operation, m_mode, matrixToEdit);
}


bool GUIManager::ShowViewEditGizmo(float* viewMatrix,float a_length)
{
	//display viewmatrix edit gizmo
	int corner = 1;
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 size = ImVec2(100, 100);
	ImVec2 windowPos = ImVec2((corner & 1) ? io.DisplaySize.x - (PADDING + size.x)  : PADDING, (corner & 2) ? io.DisplaySize.y - (PADDING + size.y) : PADDING);
	ImGuizmo::ViewManipulate(viewMatrix, a_length, windowPos, size, ImColor(0.0f, 0.0f, 0.0f, 0.4f));
	return true;
}







