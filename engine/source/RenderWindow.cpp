#include "RenderWindow.h"
#include "ShaderManager.h"
#include "Utilities.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <ext.hpp>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	RenderWindow.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 21/01/22
// Last Edited:  21/01/22
// Brief: Inherits from Application base class, facilitates drawing of scene. 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RenderWindow::RenderWindow() { m_fov = 90; }; //default fov 90
RenderWindow::~RenderWindow() {}



bool RenderWindow::onCreate()
{
	//setup clear color, depth test, culling
	m_clearColor = glm::vec3(0.95f, 0.45f, 0.75f);
	glClearColor(m_clearColor.x,m_clearColor.y,m_clearColor.z,1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearDepth(1.0);

	//set viewport
	glViewport(0, 0, m_windowWidth, m_windowHeight);

	//create camera and projection matrix
	m_cameraMatrix = glm::inverse(glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	m_projectionMatrix = glm::perspective(glm::radians(m_fov), (float)(m_windowWidth / m_windowHeight), 0.1f, 1000.0f);

	//create boids shader program 
	const unsigned int vertexShader = ShaderManager::LoadShader("resource/shaders/boid_vertex.glsl", GL_VERTEX_SHADER);
	const unsigned int fragmentShader = ShaderManager::LoadShader("resource/shaders/boid_fragment.glsl", GL_FRAGMENT_SHADER);
	m_shaderProgram = ShaderManager::CreateProgram(vertexShader, fragmentShader);

	return true;
}
 

void RenderWindow::Update(float deltaTime)
{
	//Camera
	Utilities::FreeMovement(m_cameraMatrix, deltaTime);

}

void RenderWindow::Draw() 
{
	//clear backbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

 void RenderWindow::Destroy() 
{
	ShaderManager::DestroyInstance();
	glfwTerminate();
}
