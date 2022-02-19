//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	RenderWindow.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 21/01/22
// Last Edited:  21/01/22
// Brief: Function defenitions for RenderWindow (overrides Applications). Handles tracking and drawing model data with OpenGL
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H
#include <vector>
#include <limits.h>
#include <string>
#include "Application.h"
#include "glm.hpp"


class OBJModel;
class GameObject;


class RenderWindow : public Application
{
public:
	RenderWindow();
	~RenderWindow() override;


protected:
	bool OBJSetup(std::string a_filename);
	bool onCreate() override;
	void Update(float deltaTime) override;
	void Draw() override;
	void Destroy() override;
	GameObject* CreateBoidAtRandomPosition();



private:
	//data type structs
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec4 color;
	};

	struct Line
	{
		Vertex v0;
		Vertex v1;
	};

	struct Triangle
	{
		Vertex v0;
		Vertex v1;
		Vertex v2;
	};

	struct Quad
	{
		Vertex v0;
		Vertex v1;
		Vertex v2;
		Vertex v3;
	};
	struct Polygon
	{
		std::vector<Vertex> points;
	};


	//matricies
	glm::mat4 m_cameraMatrix;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	//skybox
	unsigned int m_skyboxID;
	//editables
	glm::vec3 m_clearColor;
	float m_fov;
	//shader programs
	unsigned int m_lineProgram;
	unsigned int m_skyboxProgram;
	unsigned int m_objProgram;
	//buffers
	unsigned int m_lineVBO;
	unsigned int m_objModelBuffer[2];
	unsigned int m_skyboxVBO;
	unsigned int m_skyboxVAO;


	//data 
	Line* m_lines;
	unsigned int m_lineSize;
	glm::vec3 m_lightColor;
	OBJModel* m_objModel;


	float m_skyboxVertices[108] =
	{
		// positions          
		-2.5f,  2.5f, -2.5f,
		-2.5f, -2.5f, -2.5f,
		 2.5f, -2.5f, -2.5f,

		 2.5f, -2.5f, -2.5f,
		 2.5f,  2.5f, -2.5f,
		-2.5f,  2.5f, -2.5f,

		-2.5f, -2.5f,  2.5f,
		-2.5f, -2.5f, -2.5f,
		-2.5f,  2.5f, -2.5f,

		-2.5f,  2.5f, -2.5f,
		-2.5f,  2.5f,  2.5f,
		-2.5f, -2.5f,  2.5f,

		 2.5f, -2.5f, -2.5f,
		 2.5f, -2.5f,  2.5f,
		 2.5f,  2.5f,  2.5f,

		 2.5f,  2.5f,  2.5f,
		 2.5f,  2.5f, -2.5f,
		 2.5f, -2.5f, -2.5f,

		-2.5f, -2.5f,  2.5f,
		-2.5f,  2.5f,  2.5f,
		 2.5f,  2.5f,  2.5f,

		 2.5f,  2.5f,  2.5f,
		 2.5f, -2.5f,  2.5f,
		-2.5f, -2.5f,  2.5f,

		-2.5f,  2.5f, -2.5f,
		 2.5f,  2.5f, -2.5f,
		 2.5f,  2.5f,  2.5f,

		 2.5f,  2.5f,  2.5f,
		-2.5f,  2.5f,  2.5f,
		-2.5f,  2.5f, -2.5f,

		-2.5f, -2.5f, -2.5f,
		-2.5f, -2.5f,  2.5f,
		 2.5f, -2.5f, -2.5f,

		 2.5f, -2.5f, -2.5f,
		-2.5f, -2.5f,  2.5f,
		 2.5f, -2.5f,  2.5f
	};


};

#endif // RENDERWINDOW_H
