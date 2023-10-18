// File: Application.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/02/22
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Base application class implementing behaviour for creating and running an OpenGL App.

#include "Application.h"

#include <ctime>

#include "GUIManager.h"
#include "Utilities.h"

#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

#include "Dispatcher.h"
#include "ShaderManager.h"

auto Application::Create(const char* a_appName, unsigned int a_windowWidth, unsigned int a_windowHeight,
                         bool a_fullscreen)
-> bool
{
	//set random seed
	srand(time(nullptr));
	//create application
	if (glfwInit() == 0)
	{
		return false;
	}
	m_windowWidth = a_windowWidth;
	m_windowHeight = a_windowHeight;
	//create glfw window
	m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, a_appName,
	                            a_fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	if (m_window == nullptr)
	{
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_window);

	//initialize GLAD
	if (gladLoadGL() == 0)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}
	//get version
	std::cout << glGetString(GL_VERSION) << std::endl;

	//dispatcher 
	Dispatcher::CreateInstance();

	//callbacks
	glfwSetWindowSizeCallback(m_window, [](GLFWwindow*, int width, int height)
		{
			Dispatcher* dp = Dispatcher::GetInstance();
			if (dp != nullptr)
			{
				dp->Publish(new WindowResizeEvent(width, height), true);
			}
		});

	if (!onCreate())
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}

	//create managers

	GUIManager::CreateInstance();
	GUIManager::Init(m_window);
	return true;
	
}

void Application::Run(const char* a_name, unsigned int a_width, unsigned int a_height, bool a_fullscreen)
{
	if (Create(a_name, a_width, a_height, a_fullscreen))
	{
		Utilities::TimerReset();
		m_running = true;
		glfwSetWindowAspectRatio(m_window, 16, 9);

		do
		{
			GUIManager::NewFrame();
			GUIManager::Execute(&GUIManager::ShowFrameData, "Frame Data");
			const float deltaTime = Utilities::TimerTick();
			Update(deltaTime);
			Draw();
			GUIManager::Render();
			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}
		while (m_running && glfwWindowShouldClose(m_window) == 0);
		Destroy();
	}
	//cleanup
	glfwDestroyWindow(m_window);
	glfwTerminate();
	GUIManager::DestroyInstance();
	ShaderManager::DestroyInstance();
	Dispatcher::DestroyInstance();
}
