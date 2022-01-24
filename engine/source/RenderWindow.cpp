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

	//create shader programs
	//debug
	unsigned int vertexShader = ShaderManager::LoadShader("resource/shaders/boid_vertex.glsl", GL_VERTEX_SHADER);
	unsigned int fragmentShader = ShaderManager::LoadShader("resource/shaders/boid_fragment.glsl", GL_FRAGMENT_SHADER);
	m_baseShaderProgram = ShaderManager::CreateProgram(vertexShader, fragmentShader);

	////skybox
	//int vertexShader = ShaderManager::LoadShader("resource/shaders/skybox_vertex.glsl", GL_VERTEX_SHADER);
	//int fragmentShader = ShaderManager::LoadShader("resource/shaders/skybox_fragment.glsl", GL_FRAGMENT_SHADER);
	//m_baseShaderProgram = ShaderManager::CreateProgram(vertexShader, fragmentShader);

	////object
	//int vertexShader = ShaderManager::LoadShader("resource/shaders/object_vertex.glsl", GL_VERTEX_SHADER);
	//int fragmentShader = ShaderManager::LoadShader("resource/shaders/object_fragment.glsl", GL_FRAGMENT_SHADER);
	//m_baseShaderProgram = ShaderManager::CreateProgram(vertexShader, fragmentShader);

	//setup buffers
	//linegrid?
	glGenBuffers(1, &m_baseVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_baseVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_lines), m_lines, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//skybox
	glGenVertexArrays(1, &m_skyboxVAO);
	glBindVertexArray(m_skyboxVAO);
	glGenBuffers(1, &m_skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyboxVertices), m_skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//object

	return true;
}
 

void RenderWindow::Update(float deltaTime)
{
	//Camera
	Utilities::FreeMovement(m_cameraMatrix, deltaTime);

}

void RenderWindow::Draw() 
{
	//SETUP

	//clear backbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//enable shader program
	glUseProgram(m_baseShaderProgram);
	//get view matrix
	m_viewMatrix = glm::inverse(m_cameraMatrix);
	//setup pvm
	glm::mat4 projectionViewMatrix = m_projectionMatrix * m_viewMatrix;
	//send pvm to shader
	int projectionViewMatrixUniformLocation = glGetUniformLocation(m_baseShaderProgram, "ProjectionViewMatrix");
	glUniformMatrix4fv(projectionViewMatrixUniformLocation, 1, false, glm::value_ptr(projectionViewMatrix));

	//LINE GRID

}

 void RenderWindow::Destroy() 
{
	ShaderManager::DestroyInstance();
	glfwTerminate();
}
