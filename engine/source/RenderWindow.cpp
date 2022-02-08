#include "RenderWindow.h"
#include "ShaderManager.h"
#include "Utilities.h"
#include "obj_loader.h"
#include "TextureManager.h"

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

#define MAX_RENDERED_ENTITIES = 25;

bool RenderWindow::onCreate()
{
	TextureManager* pTexM = TextureManager::GetInstance();
	//handle skybox
	std::string path = "./resource/skyboxes/";
	std::string skyboxName = "skybox_1"; //testing, should be variable
	m_skyboxID = pTexM->LoadTexture((path + skyboxName).c_str(), true);

	//default lighting
	m_lightColor = glm::vec3(0);

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

	//set shader programs
	//obj
	GLuint vertexShader = ShaderManager::LoadShader("resource/shaders/obj_vertex.glsl", GL_VERTEX_SHADER);
	GLuint fragmentShader = ShaderManager::LoadShader("resource/shaders/obj_fragment.glsl", GL_FRAGMENT_SHADER);
	m_objProgram = ShaderManager::CreateProgram(vertexShader, fragmentShader);


	//skybox
	vertexShader = ShaderManager::LoadShader("resource/shaders/skybox_vertex.glsl", GL_VERTEX_SHADER);
	fragmentShader = ShaderManager::LoadShader("resource/shaders/skybox_fragment.glsl", GL_FRAGMENT_SHADER);
	m_skyboxProgram = ShaderManager::CreateProgram(vertexShader, fragmentShader);

	//lines
	vertexShader = ShaderManager::LoadShader("resource/shaders/ui_vertex.glsl", GL_VERTEX_SHADER);
	fragmentShader = ShaderManager::LoadShader("resource/shaders/ui_fragment.glsl", GL_FRAGMENT_SHADER);
	m_lineProgram = ShaderManager::CreateProgram(vertexShader, fragmentShader);
	m_lineSize = 42;
	m_lines = new Line[m_lineSize];

	//generate line grid
	for (unsigned int i = 0; i < m_lineSize / 2; ++i)
	{
		glm::vec4 color = (i == 10) ? glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		int j = i * 2;
		m_lines[j].v0.pos = glm::vec3(i - 10.0f, 0.0f, 10.0f);
		m_lines[j].v1.pos = glm::vec3(i - 10.0f, 0.0f, -10.0f);

		m_lines[j].v0.color = color;
		m_lines[j].v1.color = color;

		m_lines[j + 1].v0.pos = glm::vec3(10.0f, 0.0f, i - 10.0f);
		m_lines[j + 1].v1.pos = glm::vec3(-10.0f, 0.0f, i - 10.0f);

		m_lines[j + 1].v0.color = color;
		m_lines[j + 1].v1.color = color;
	}

	//generate buffers
	//lines
	glGenBuffers(1, &m_lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glBufferData(GL_ARRAY_BUFFER, m_lineSize * sizeof(Line), m_lines, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//skybox

	glGenVertexArrays(1, &m_skyboxVAO);
	glBindVertexArray(m_skyboxVAO);
	glGenBuffers(1, &m_skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyboxVertices), m_skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//model
	glGenBuffers(2, m_objModelBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_objModelBuffer[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_objModelBuffer[1]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//load default boid model
	return OBJSetup("default.obj");

}
 

bool RenderWindow::OBJSetup(std::string a_filename)
{
	a_filename = "./resource/obj_models/" + a_filename;
	m_objModel = OBJLoader::OBJProcess(a_filename,false);
	if (m_objModel == nullptr) return false;

	//handle textures
	TextureManager* pTexM = TextureManager::CreateInstance();
	for (unsigned int i = 0; i < m_objModel->GetMaterialCount(); ++i)
	{
		OBJMaterial* currentMaterial = m_objModel->GetMaterial(i);
		for (int j = 0; j < OBJMaterial::TextureTypes_Count; ++j)
		{
			if (!currentMaterial->textureFileNames[j].empty())
			{
				currentMaterial->textureIDs[j] = pTexM->LoadTexture(currentMaterial->textureFileNames[j].c_str());
			}
		}
	}
}

void RenderWindow::Update(float deltaTime)
{
	//Camera
	Utilities::FreeMovement(m_cameraMatrix, deltaTime);
	//Entities


}

void RenderWindow::Draw() 
{
	//clear backbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//enable shaders
	glUseProgram(m_lineProgram);
	//get view matrix from camera matrix in world space 
	m_viewMatrix = glm::inverse(m_cameraMatrix);
	glm::mat4 projectionViewMatrix = m_projectionMatrix * m_viewMatrix;
	//get projection view matrix var location from shader file
	int projectionViewMatrixUniformLocation = glGetUniformLocation(m_lineProgram, "ProjectionViewMatrix");
	//send pointer to location of matrix 
	glUniformMatrix4fv(projectionViewMatrixUniformLocation, 1, false, glm::value_ptr(projectionViewMatrix));
	//enable array attribs
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//tell opengl where array is, no of element components, data type, normalization
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)nullptr + sizeof(float) * 3); //x3 because vec3 used for position data
	//draw line grid
	glDrawArrays(GL_LINES, 0, (m_lineSize * 2) + 10);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//light position 
	glm::vec3 lightPos;
	double time = glfwGetTime();
	lightPos.x = sin(time) * 5.0f;
	lightPos.y = sin(time / 2.0f);
	lightPos.z = 10.0f;

	//TODO: DRAW MODELS HERE

	
	
	//draw skybox
	glUseProgram(m_skyboxProgram);
	projectionViewMatrix = glm::mat4(glm::mat3(projectionViewMatrix)); //remove translation
	projectionViewMatrixUniformLocation = glGetUniformLocation(m_skyboxProgram, "ProjectionViewMatrix");
	glUniformMatrix4fv(projectionViewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionViewMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	glBindVertexArray(m_skyboxVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxID);
	glDepthFunc(GL_LEQUAL);
	glFrontFace(GL_CW);
	glDrawArrays(GL_TRIANGLES, 0, 36); //108 floats in skyboxverticies/3 (each vertex is 3 floats)
	glFrontFace(GL_CCW);
	glDepthFunc(GL_LESS);
	glBindVertexArray(0);
	//release program
	glUseProgram(0);
	
}

 void RenderWindow::Destroy() 
{
	 delete m_objModel;
	 delete[] m_lines;
	 glDeleteVertexArrays(1, &m_skyboxVAO);
	 glDeleteBuffers(1, &m_lineVBO);
	 glDeleteBuffers(1, &m_skyboxVBO);
	 glDeleteBuffers(2, &m_objModelBuffer[0]);
	 ShaderManager::DestroyInstance();
	 TextureManager::DestroyInstance();
	 glfwTerminate();
}
