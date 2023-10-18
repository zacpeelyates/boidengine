// File: GUIManager.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/05/01
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Singleton class implementation handling IMGUI and IMGUIZMO library usage. Draw UI to screen each frame.
// May edit some values passed in if called UI element directly modifies values. Most UI elements return a bool
// determining whether or not they were used / interacted with this frame, which can be used by the calling class
// (see RenderWindow.cpp)

#include "GUIManager.h"
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>


const int GUIManager::PADDING = 10;
GUIManager* GUIManager::m_instance = nullptr;
//default values for imguizmo operation and mode
ImGuizmo::OPERATION GUIManager::m_operation = ImGuizmo::ROTATE;
//world mode always used, no parenting structure for local transforms currently
ImGuizmo::MODE GUIManager::m_mode = ImGuizmo::WORLD;

//singleton pattern implementation 
auto GUIManager::GetInstance() -> GUIManager*
{
	return m_instance == nullptr ? CreateInstance() : m_instance;
}

auto GUIManager::CreateInstance() -> GUIManager*
{
	if (m_instance == nullptr)
	{
		m_instance = new GUIManager();
	}
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


auto GUIManager::Init(GLFWwindow* window) -> bool
{
	//imgui setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	//if setup failed return false 
	if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
	{
		return false;
	}
	if (!ImGui_ImplOpenGL3_Init("#version 400"))
	{
		return false;
	}
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
	m_instance->m_yOffset = PADDING;
	m_instance->m_xOffset = PADDING;
}

void GUIManager::Render() //ImGUI render 
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUIManager::ShowFrameData()
{
	const ImGuiIO& io = ImGui::GetIO();
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
	const ImGuiIO& io = ImGui::GetIO();
	if (m_yOffset + PADDING >= io.DisplaySize.y  )
	{
		//create additional column 
		m_yOffset = 0;
		m_xOffset += io.DisplaySize.x / m_columns;
	}
	const auto windowPos = ImVec2((m_corner & 1) != 0 ? io.DisplaySize.x - m_xOffset : m_xOffset,
	                              (m_corner & 2) != 0 ? io.DisplaySize.y - m_yOffset : m_yOffset);
	const auto windowPivot = ImVec2((m_corner & 1) != 0 ? 1.0F : 0.0F, (m_corner & 2) != 0 ? 1.0F : 0.0F);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, windowPivot);
	ImGui::SetNextWindowBgAlpha(0.4F);
}


auto GUIManager::ShowFileLoader(std::string& input) -> bool
{
	//show file loading GUI element
	ImGui::InputText("Filename", &m_inputBuffer);
	if (!m_inputBuffer.empty())
	{
		input = m_inputBuffer;
	}
	return ImGui::Button("Load");
}

auto GUIManager::ShowLoadedFileList(std::vector<std::string> loadedFiles, std::string& selectedFile) -> bool
{
	for (auto& loadedFile : loadedFiles)
	{
		bool selected = false;
		if (ImGui::Selectable(loadedFile.c_str(), &selected, ImGuiSelectableFlags_DontClosePopups, ImVec2(200, 15)))
		{
			if (selected)
			{
				selectedFile = loadedFile;
				return true;
			}
		}
	}
	return false;
}

auto GUIManager::ShowColorEditor(float* firstElement, const bool alpha, const char* title) -> bool
{
	return alpha ? ImGui::ColorEdit4(title, firstElement) : ImGui::ColorEdit3(title, firstElement);
}

auto GUIManager::ShowSlider(float* valueToEdit, const float a_min, const float a_max) -> bool
{
	return ImGui::SliderFloat("##defaultSlider", valueToEdit, a_min, a_max);
}

auto GUIManager::ShowSliderInt(int* valueToEdit, const int a_min, int a_max) -> bool
{
	return ImGui::SliderInt("##defaultSlider", valueToEdit, a_min, a_max);
}

auto GUIManager::ShowMatrixEditWindow(float matrixToEdit[16], const float viewMatrix[16], const float projectionMatrix[16])-> bool
{
	bool b = false;
	//store matrix in easier to edit arrays
	float translation[3];
	float rotation[3];
	float scale[3];
	ImGuizmo::DecomposeMatrixToComponents(matrixToEdit, translation, rotation, scale);

	ImGui::Text("    X         Y         Z    "); //probably the worst possible way to do this but oh well

	//show grid of float to edit arrays
	if (ImGui::InputFloat3("Translation", translation, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
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
		Manipulate(viewMatrix, projectionMatrix, m_operation, m_mode, matrixToEdit);
	}
	return b;
}

auto GUIManager::ShowMatrixEditGizmo(float matrixToEdit[16], const float viewMatrix[16], const float projectionMatrix[16]) -> bool
{
	//click/drag matrix manipulation
	const ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
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
	return Manipulate(viewMatrix, projectionMatrix, m_operation, m_mode, matrixToEdit);
}


auto GUIManager::ShowViewEditGizmo(float* viewMatrix, float a_length) -> bool
{
	//display viewmatrix edit gizmo
	const int corner = 1;
	const ImGuiIO& io = ImGui::GetIO();
	const auto size = ImVec2(100, 100);
	const auto windowPos = ImVec2(io.DisplaySize.x - (PADDING + size.x), PADDING);
	ImGuizmo::ViewManipulate(viewMatrix, a_length, windowPos, size, ImColor(0.0F, 0.0F, 0.0F, 0.4F));
	return true;
}

auto GUIManager::ShowVectorEditor(float* valueToEdit) -> bool
{
	return ImGui::InputFloat3("##InvokedVecEdit", valueToEdit);
}
