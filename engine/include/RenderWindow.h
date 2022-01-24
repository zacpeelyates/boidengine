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
#include "Application.h"
#include "glm.hpp"

class RenderWindow : public Application
{
public:
	RenderWindow();
	~RenderWindow() override;


protected:
	bool onCreate() override;
	void Update(float deltaTime) override;
	void Draw() override;
	void Destroy() override;
private:
	//data type structs

	//member data structs
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
	//programs
	unsigned int m_baseShaderProgram;
	//editables
	glm::vec3 m_clearColor;
	float m_fov;
	//Vertex  Objects
	unsigned int m_baseVBO;
	unsigned int m_skyboxVAO;
	unsigned int m_skyboxVBO;


	//data 
	Line* m_lines;



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
