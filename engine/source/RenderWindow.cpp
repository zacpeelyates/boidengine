// File: RenderWindow.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/05/01
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Inherits from Application base class, facilitates drawing of scene. 

#include "RenderWindow.h"
#include "GUIManager.h"
#include "GameObject.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Utilities.h"
#include "obj_Loader.h"

#include <GLFW/glfw3.h>
#include <Octree.h>
#include <ext.hpp>
#include <glad/glad.h>
#include <glm.hpp>


#include "BoidBehaviour.h"
#include "Dispatcher.h"
#include <BoxCollider.h>
#include <Gizmos.h>
#include <iostream>
#include <Transform.h>


#define MAX_BOIDS 250


RenderWindow::RenderWindow() { numBoids = MAX_BOIDS / 2; }
RenderWindow::~RenderWindow() = default;

void RenderWindow::onWindowResize(WindowResizeEvent* e)
{
	if (e->GetWidth() > 0 && e->GetHeight() > 0) { //fixed crash when making window tiny
		m_windowWidth = e->GetWidth();
		m_windowHeight = e->GetHeight();
		e->Handled();
		std::cout << "Window Resize Event" << std::endl << "w: " << m_windowWidth << " h: " << m_windowHeight << std::endl;
		m_projectionMatrix = glm::perspective(glm::radians(m_fov), static_cast<float>(m_windowWidth / m_windowHeight), 0.1f, 1000.0f);
		glViewport(0, 0, m_windowWidth, m_windowHeight);
	}
};

auto RenderWindow::onCreate() -> bool
{
	Dispatcher* dp = Dispatcher::GetInstance();
	if (dp != nullptr)
	{
		dp->Subscribe(this, &RenderWindow::onWindowResize);
	}

	TextureManager* pTexM = TextureManager::GetInstance();
	//handle skybox
	std::string path = "./resource/skyboxes/";
	std::string skyboxName = "skybox_1"; //testing, should be variable
	m_skyboxID = pTexM->LoadTexture((path + skyboxName).c_str(), true);

	//default lighting
	m_lightColor = glm::vec3(0);

	//setup clear color, depth test, culling
	m_clearColor = glm::vec3(0.95F, 0.45F, 0.75F);
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, 1.0F);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glClearDepth(1.0);

	//set viewport
	glViewport(0, 0, m_windowWidth, m_windowHeight);

	//create camera and projection matrix
	m_cameraMatrix = inverse(lookAt(glm::vec3(0, 20, 30), glm::vec3(0, 10, 0), glm::vec3(0, 1, 0)));
	m_projectionMatrix = glm::perspective(glm::radians(m_fov), static_cast<float>(m_windowWidth / m_windowHeight), 0.1F,
	                                      1000.0F);

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
		glm::vec4 color = i == 10 ? glm::vec4(1.0F, 1.0F, 1.0F, 1.0F) : glm::vec4(0.0F, 0.0F, 0.0F, 1.0F);

		int j = i * 2;
		m_lines[j].v0.pos = glm::vec3(i - 10.0F, 0.0F, 10.0F);
		m_lines[j].v1.pos = glm::vec3(i - 10.0F, 0.0F, -10.0F);

		m_lines[j].v0.color = color;
		m_lines[j].v1.color = color;

		m_lines[j + 1].v0.pos = glm::vec3(10.0F, 0.0F, i - 10.0F);
		m_lines[j + 1].v1.pos = glm::vec3(-10.0F, 0.0F, i - 10.0F);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof m_skyboxVertices, m_skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//model
	glGenBuffers(2, m_objModelBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_objModelBuffer[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_objModelBuffer[1]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//create boid objects
	BoidBehaviour::SetupForceList();

	for (unsigned int i = 0; i < numBoids; ++i)
	{
		CreateBoidAtRandomPosition();
	}

	Gizmos::create();
	BoidBehaviour::CreatePartitions();


	return OBJSetup("simple.obj"); //default.obj for textured birds - simple.obj for cones
}

auto RenderWindow::OBJSetup(std::string a_filename) -> bool
{
	a_filename = "./resource/obj_models/" + a_filename;
	m_objModel = OBJLoader::OBJProcess(a_filename, false, 5);
	if (m_objModel == nullptr)
	{
		return false;
	}

	//handle textures
	TextureManager* pTexM = TextureManager::CreateInstance();
	ShaderManager::CreateInstance();
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
	return true;
}

void RenderWindow::Update(float deltaTime)
{
	//Camera
	Utilities::FreeMovement(m_cameraMatrix, deltaTime, 10);

	BoidBehaviour::UpdateBoidNeighbourLists();

	//Objects
	for (GameObject* g : GameObject::s_GameObjects)
	{
		g->Update(deltaTime);
	}

	int temp = numBoids;
	//GUI
	GUIManager::Execute<float*>(&GUIManager::ShowVectorEditor, value_ptr(BoidBehaviour::seekPoint), "Seek Point");
	GUIManager::Execute<float*>(&GUIManager::ShowVectorEditor, value_ptr(BoidBehaviour::fleePoint), "Flee Point");
	if(GUIManager::Execute<int*, int, int>(&GUIManager::ShowSliderInt, &numBoids, 1, MAX_BOIDS, "Boid Count"))
	{
		//if value is changed
		if (numBoids > temp) for (int i = 0; i < numBoids - temp; ++i)
		{
			Octree::root->Emplace(CreateBoidAtRandomPosition()); //create new boids until we have the new number
		}
		else if (numBoids < temp) for (int i = 0; i < temp - numBoids; ++i)
		{
			GameObject::s_GameObjects.pop_back(); //remove boids until we have new number
		}
		BoidBehaviour::UpdateBoidNeighbourLists();
	}


	for (BoidForce* current = BoidBehaviour::s_head; current != nullptr; current = current->next)
	{
		GUIManager::Execute<float*, float, float>(&GUIManager::ShowSlider, &current->m_weight, 0, 2,current->m_name.c_str());
		if (current->params != nullptr) //if we have additional params to edit
		{
			for (int i = 0; i < current->numParams; ++i)
			//create sliders for additional params
			{
				ForceParams p = current->params[i];
				GUIManager::Execute<float*, float, float>(&GUIManager::ShowSlider, p.value, p.min, p.max, p.name.c_str());
			}
		}
	}
}

void RenderWindow::Draw()
{
	//clear backbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//enable shaders
	glUseProgram(m_lineProgram);
	//clear gizmos
	Gizmos::clear();
	//get view matrix from camera matrix in world space 
	m_viewMatrix = inverse(m_cameraMatrix);
	glm::mat4 projectionViewMatrix = m_projectionMatrix * m_viewMatrix;
	//get projection view matrix var location from shader file
	int projectionViewMatrixUniformLocation = glGetUniformLocation(m_lineProgram, "ProjectionViewMatrix");
	//send pointer to location of matrix 
	glUniformMatrix4fv(projectionViewMatrixUniformLocation, 1, 0u, value_ptr(projectionViewMatrix));
	//enable array attribs
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//tell opengl where array is, no of element components, data type, normalization
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<char*>(nullptr) + sizeof(float) * 3);
	//x3 because vec3 used for position data
	//draw line grid
	glDrawArrays(GL_LINES, 0, m_lineSize * 2 + 10);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//light position 
	glm::vec3 lightPos;
	double time = glfwGetTime();
	lightPos.x = sin(time) * 5.0F;
	lightPos.y = sin(time / 2.0F);
	lightPos.z = 10.0F;

	glUseProgram(m_objProgram);

	int cameraPositionUniformLocation = glGetUniformLocation(m_objProgram, "camPos");
	glUniform3fv(cameraPositionUniformLocation, 1, value_ptr(m_cameraMatrix[3]));

	int lightPos_location = glGetUniformLocation(m_objProgram, "lightPos");
	glUniform3fv(lightPos_location, 1, value_ptr(lightPos));
	int lightColor_location = glGetUniformLocation(m_objProgram, "lightColor");
	glUniform4fv(lightColor_location, 1, value_ptr(m_lightColor));

	Octree::Draw();
	Gizmos::addBox(BoidBehaviour::seekPoint, glm::vec3(1), true, glm::vec4(0, 255, 0, 255));
	Gizmos::addBox(BoidBehaviour::fleePoint, glm::vec3(1), true, glm::vec4(255, 0, 0, 255));

	//draw gameobjects
	for (GameObject* g : GameObject::s_GameObjects)
	{
		g->Draw(m_objProgram);
	}

	//draw gizmos
	if (m_drawGizmos)
	{
		Gizmos::draw(projectionViewMatrix);
	}


	//draw OBJ model
	if (m_objModel != nullptr)
	{
		glUseProgram(m_objProgram);
		projectionViewMatrixUniformLocation = glGetUniformLocation(m_objProgram, "ProjectionViewMatrix");
		//send pointer to location of matrix 
		glUniformMatrix4fv(projectionViewMatrixUniformLocation, 1, false, value_ptr(projectionViewMatrix));


		int cameraPositionUniformLocation = glGetUniformLocation(m_objProgram, "camPos");
		glUniform3fv(cameraPositionUniformLocation, 1, value_ptr(m_cameraMatrix[3]));

		int kA_location = glGetUniformLocation(m_objProgram, "kA");
		int kD_location = glGetUniformLocation(m_objProgram, "kD");
		int kS_location = glGetUniformLocation(m_objProgram, "kS");
		int nS_location = glGetUniformLocation(m_objProgram, "nS");
		int TextureUniformLocation = 0;
		int MatrixListUniformLocation = glGetUniformLocation(m_objProgram, "matrixList");

		std::vector<glm::mat4> matrixList;
		//create list of positions for us to draw with glDrawElementsInstanced, means we only need to load the boid model once
		matrixList.reserve(GameObject::s_GameObjects.size());
		for (GameObject* s_GameObject : GameObject::s_GameObjects)
		{
			matrixList.push_back(s_GameObject->GetComponent<Transform>()->Orthonormalize());
		}


		for (int i = 0; i < m_objModel->GetMeshCount(); ++i)
		{
			OBJMesh* currentMesh = m_objModel->GetMesh(i);


			int lightPos_location = glGetUniformLocation(m_objProgram, "lightPos");
			glUniform3fv(lightPos_location, 1, value_ptr(lightPos));
			int lightColor_location = glGetUniformLocation(m_objProgram, "lightColor");
			glUniform4fv(lightColor_location, 1, value_ptr(m_lightColor));


			OBJMaterial* currentMaterial = currentMesh->m_activeMaterial;
			bool bUseNormal = false;
			bool bUseSpecular = false;
			bool bUseDiffuse = false;

			if (currentMaterial != nullptr)
			{
				glUniform3fv(kA_location, 1, value_ptr(currentMaterial->GetAmbience()));
				glUniform3fv(kD_location, 1, value_ptr(currentMaterial->GetDiffuse()));
				glUniform3fv(kS_location, 1, value_ptr(currentMaterial->GetSpecular()));
				glUniform1f(nS_location, currentMaterial->GetSpecularExponent());


				//textures
				if (!currentMaterial->textureFileNames[OBJMaterial::DiffuseTexture].empty())
				{
					//diffuse
					bUseDiffuse = true;
					TextureUniformLocation = glGetUniformLocation(m_objProgram, "DiffuseTexture");
					glUniform1i(TextureUniformLocation, 0);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, currentMaterial->textureIDs[OBJMaterial::DiffuseTexture]);
				}

				if (!currentMaterial->textureFileNames[OBJMaterial::SpecularTexture].empty())
				{
					//specular
					bUseSpecular = true;
					TextureUniformLocation = glGetUniformLocation(m_objProgram, "SpecularTexture");
					glUniform1i(TextureUniformLocation, 1);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, currentMaterial->textureIDs[OBJMaterial::SpecularTexture]);
				}

				if (!currentMaterial->textureFileNames[OBJMaterial::NormalTexture].empty())
				{
					//normal
					bUseNormal = true;
					TextureUniformLocation = glGetUniformLocation(m_objProgram, "NormalTexture");
					glUniform1i(TextureUniformLocation, 2);
					glActiveTexture(GL_TEXTURE2);
					glBindTexture(GL_TEXTURE_2D, currentMaterial->textureIDs[OBJMaterial::NormalTexture]);
				}
			}
			else
			{
				//use default lighting
				glUniform3fv(kA_location, 1, value_ptr(glm::vec3(0.25F)));
				glUniform3fv(kD_location, 1, value_ptr(glm::vec3(0.25F)));
				glUniform3fv(kS_location, 1, value_ptr(glm::vec3(0.25F)));
				glUniform1f(nS_location, 1.0F);
			}

			int TextureCheckUniformLocation = glGetUniformLocation(m_objProgram, "useNormal");
			glUniform1i(TextureCheckUniformLocation, static_cast<GLint>(bUseNormal));
			TextureCheckUniformLocation = glGetUniformLocation(m_objProgram, "useSpecular");
			glUniform1i(TextureCheckUniformLocation, static_cast<GLint>(bUseSpecular));
			TextureCheckUniformLocation = glGetUniformLocation(m_objProgram, "useDiffuse");
			glUniform1i(TextureCheckUniformLocation, static_cast<GLint>(bUseDiffuse));

			//bind buffers
			glBindBuffer(GL_ARRAY_BUFFER, m_objModelBuffer[0]);
			glBufferData(GL_ARRAY_BUFFER, currentMesh->m_verts.size() * sizeof(OBJVertex), currentMesh->m_verts.data(),
			             GL_STATIC_DRAW);

			glEnableVertexAttribArray(0); //position
			glEnableVertexAttribArray(1); //normal
			glEnableVertexAttribArray(2); //UVs

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OBJVertex),
			                      static_cast<char*>(nullptr) + OBJVertex::Offsets::POS); //vec3 pos
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(OBJVertex),
			                      static_cast<char*>(nullptr) + OBJVertex::Offsets::NORMAL); //vec3 normal
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(OBJVertex),
			                      static_cast<char*>(nullptr) + OBJVertex::Offsets::UVCOORD); //vec2 UVs

			const GLfloat* data = value_ptr(matrixList[0]);
			glUniformMatrix4fv(MatrixListUniformLocation, GameObject::s_GameObjects.size(), 0u, data);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_objModelBuffer[1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentMesh->m_indicies.size() * sizeof(unsigned int),
			             currentMesh->m_indicies.data(), GL_STATIC_DRAW);
			glDrawElementsInstanced(GL_TRIANGLES, currentMesh->m_indicies.size(), GL_UNSIGNED_INT, nullptr,
			                        GameObject::s_GameObjects.size());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		//unbind buffer/arrays 
		glDisableVertexAttribArray(0); //position
		glDisableVertexAttribArray(1); //normal
		glDisableVertexAttribArray(2); //uvcoord
	}

	//draw skybox
	glEnable(GL_CULL_FACE); //enable culling for skybox only
	glUseProgram(m_skyboxProgram);
	projectionViewMatrix = glm::mat4(glm::mat3(projectionViewMatrix)); //remove translation
	projectionViewMatrixUniformLocation = glGetUniformLocation(m_skyboxProgram, "ProjectionViewMatrix");
	glUniformMatrix4fv(projectionViewMatrixUniformLocation, 1, GL_FALSE, value_ptr(projectionViewMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	glBindVertexArray(m_skyboxVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, static_cast<void*>(nullptr));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxID);
	glDepthFunc(GL_LEQUAL);
	glFrontFace(GL_CW);
	glDrawArrays(GL_TRIANGLES, 0, 36); //108 floats in skyboxverticies/3 (each vertex is 3 floats)
	glDisable(GL_CULL_FACE); //disable culling after draw call
	glFrontFace(GL_CCW);
	glDepthFunc(GL_LESS);
	glBindVertexArray(0); //unbind
	glUseProgram(0); //release program
}

void RenderWindow::Destroy()
{
	delete[] m_lines;
	for (const GameObject* g : GameObject::s_GameObjects)
	{
		delete g;
	}
	glDeleteVertexArrays(1, &m_skyboxVAO);
	glDeleteBuffers(1, &m_lineVBO);
	glDeleteBuffers(1, &m_skyboxVBO);
	glDeleteBuffers(2, &m_objModelBuffer[0]);
	ShaderManager::DestroyInstance();
	TextureManager::DestroyInstance();
	glfwTerminate();
}

auto RenderWindow::CreateBoidAtRandomPosition() -> GameObject*
{
	auto* const boid = new GameObject();
	const std::shared_ptr<Transform> t = boid->AddComponent<Transform>();
	const float radius = 75.0f;
	t->SetTranslation(glm::ballRand(radius));
	boid->AddComponent<BoxCollider>();
	boid->AddComponent<BoidBehaviour>();
	boid->Init();

	return boid;
}
