// File: RenderWindow.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/05/01
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Main Application window for BoidEngine, extends Generic Application Window

#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H
#include <vector>
#include <string>
#include "Application.h"
#include "Event.h"
#include "glm.hpp"


class OBJModel;
class GameObject;


class RenderWindow : public Application
{
public:
	RenderWindow();
	~RenderWindow() override;
	void onWindowResize(WindowResizeEvent* e);


protected:
	bool OBJSetup(std::string a_filename);
	bool onCreate() override;
	void Update(float deltaTime) override;
	void Draw() override;
	void Destroy() override;
	static GameObject* CreateBoidAtRandomPosition();



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
	glm::mat4 m_cameraMatrix{};
	glm::mat4 m_projectionMatrix{};
	glm::mat4 m_viewMatrix{};
	//skybox
	unsigned int m_skyboxID{};
	//editables
	glm::vec3 m_clearColor;
	float m_fov{90};
	int numBoids;
	//shader programs
	unsigned int m_lineProgram;
	unsigned int m_skyboxProgram;
	unsigned int m_objProgram;
	//buffers
	unsigned int m_lineVBO;
	unsigned int m_objModelBuffer[2];
	unsigned int m_skyboxVBO;
	unsigned int m_skyboxVAO;

	//gizmos
	bool m_drawGizmos{true};

	//data / editables
	Line* m_lines;
	unsigned int m_lineSize;
	glm::vec3 m_lightColor;
	OBJModel* m_objModel;


	//there gotta be a better way of doing this but I copy pasted it from the skybox tutorial we were given
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
