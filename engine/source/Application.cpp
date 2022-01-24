#include "Application.h"
#include "Utilities.h"
#include "GUIManager.h"

#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	Application.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 21/01/22
// Last Edited:  21/01/22
// Brief: Base application class implementing behaviour for creating and running an OpenGL App.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Application::Create(const char* a_appName, unsigned int a_windowWidth, unsigned int a_windowHeight, bool a_fullscreen)
{
	//create application
	if (!glfwInit()) return false;
	m_windowWidth = a_windowWidth;
	m_windowHeight = a_windowHeight;
	//create glfw window
	m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, a_appName, (a_fullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
	if (!m_window)
	{
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_window);

	//initialize GLAD
	if (!gladLoadGL())
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}
	//get version
	std::cout << glGetString(GL_VERSION) << std::endl;
	
	if (!onCreate())
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}

	//create manager instances
	if(!GUIManager::CreateInstance()->Init(m_window)) return false;

	return true;
}

void Application::Run(const char* a_name, unsigned int a_width, unsigned int a_height, bool a_fullscreen)
{
	
	if (Create(a_name, a_width, a_height, a_fullscreen))
	{
		Utilities::TimerReset();
		m_running = true;
		glfwSetWindowAspectRatio(m_window, 16, 9);
		GUIManager* gui = GUIManager::GetInstance();
		
		do
		{
			gui->NewFrame();
			gui->ShowFrameData();
			float deltaTime = Utilities::TimerTick();
			Update(deltaTime);
			Draw();
			gui->Render();

			glfwSwapBuffers(m_window);
			glfwPollEvents();

		} while (m_running && glfwWindowShouldClose(m_window) == 0);
		Destroy();
	}
	//cleanup
	glfwDestroyWindow(m_window);
	glfwTerminate();
	GUIManager::DestroyInstance();
}


