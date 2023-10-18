// File: Utilities.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/04/11
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Various utilities that couldn't find a home anywhere else :(

#include "Utilities.h"
#include "fstream"
#include <GLFW/glfw3.h>
#include <ext.hpp> 
#include <glm.hpp>

static double s_prevTime = 0;
static float s_totalTime = 0;
static float s_deltaTime = 0;

//time function definitions

void Utilities::TimerReset()
{
	//reset timer statics
	s_prevTime = glfwGetTime();
	s_totalTime = 0;
	s_deltaTime = 0;
}

auto Utilities::TimerTick() -> float
{
	//incrememnt totaltime, return new deltatime
	const double currentTime = glfwGetTime();
	s_deltaTime = static_cast<float>(currentTime - s_prevTime);
	s_totalTime += s_deltaTime;
	s_prevTime = currentTime;
	return s_deltaTime;
}

auto Utilities::getDeltaTime() -> float
{
	return s_deltaTime;
}

auto Utilities::getTotalTime() -> float
{
	return s_totalTime;
}

//file loading definitions 

auto Utilities::FileToBuffer(const std::string& a_strFilePath) -> char*
{
	std::streamsize temp = 0;
	return FileToBuffer(a_strFilePath, temp);
}


auto Utilities::FileToBuffer(const std::string& a_strFilePath, std::streamsize& a_rFileSize) -> char*
{
	//load file into char* buffer
	std::fstream file;
	file.open(a_strFilePath, std::ios_base::in | std::ios_base::binary);
	if (file.is_open())
	{
		file.ignore(std::numeric_limits<std::streamsize>::max());
		a_rFileSize = file.gcount(); //store filesize in passed buffer
		file.clear();
		file.seekg(0, std::ios_base::beg);
		if (a_rFileSize == 0)
		{
			//early exit if file is empty
			file.close();
			return nullptr;
		}
		auto* databuffer = new char[a_rFileSize + 1]; //create appropriately-sized buffer
		memset(databuffer, 0, a_rFileSize + 1); //clear buffer
		file.read(databuffer, a_rFileSize); //read data into buffer
		file.close();
		return databuffer;
	}
	return nullptr;
}

//camera function
void Utilities::FreeMovement(glm::mat4& a_m4Transform, float a_fDeltaTime, float a_fSpeed, const glm::vec3& a_v3Up)
{
	GLFWwindow* window = glfwGetCurrentContext();

	//get camera transform vectors
	glm::vec4 v4Right = a_m4Transform[0];
	glm::vec4 v4Up = a_m4Transform[1];
	glm::vec4 v4Forward = a_m4Transform[2];
	//get location vectors
	glm::vec4 v4Translation = a_m4Transform[3];
	//set speed variables (shift to sprint)
	float defactoSpeed = a_fDeltaTime * a_fSpeed;
	float framespeed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? defactoSpeed * 2.0F : defactoSpeed;

	//movement / translation
	if (glfwGetKey(window, 'W') == GLFW_PRESS)
	{
		v4Translation -= v4Forward * framespeed;
	}
	if (glfwGetKey(window, 'S') == GLFW_PRESS)
	{
		v4Translation += v4Forward * framespeed;
	}
	if (glfwGetKey(window, 'A') == GLFW_PRESS)
	{
		v4Translation -= v4Right * framespeed;
	}
	if (glfwGetKey(window, 'D') == GLFW_PRESS)
	{
		v4Translation += v4Right * framespeed;
	}
	if (glfwGetKey(window, 'Q') == GLFW_PRESS)
	{
		v4Translation += v4Up * framespeed;
	}
	if (glfwGetKey(window, 'E') == GLFW_PRESS)
	{
		v4Translation -= v4Up * framespeed;
	}

	a_m4Transform[3] = v4Translation;

	//rotation
	static bool sbMouse2ButtonDown = false;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		static double sdPrevMouseX = 0;
		static double sdPrevMouseY = 0;
		if (!sbMouse2ButtonDown)
		{
			sbMouse2ButtonDown = true;
			glfwGetCursorPos(window, &sdPrevMouseX, &sdPrevMouseY);
		}
		//get mouse movement delta 
		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		double dDeltaX = mouseX - sdPrevMouseX;
		double dDeltaY = mouseY - sdPrevMouseY;

		sdPrevMouseX = mouseX;
		sdPrevMouseY = mouseY;


		glm::mat4 m4Temp;
		if (dDeltaY != 0)
		{
			//set temp to pitch
			m4Temp = axisAngleMatrix(v4Right.xyz(), static_cast<float>(-dDeltaY) / 150.0F);
			v4Right = m4Temp * v4Right;
			v4Up = m4Temp * v4Up;
			v4Forward = m4Temp * v4Forward;
		}

		if (dDeltaX != 0)
		{
			//set temp to yaw
			m4Temp = axisAngleMatrix(a_v3Up, static_cast<float>(-dDeltaX) / 150.0F);
			v4Right = m4Temp * v4Right;
			v4Up = m4Temp * v4Up;
			v4Forward = m4Temp * v4Forward;
		}
		//assign new values to transformed matrix 
		a_m4Transform[0] = v4Right;
		a_m4Transform[1] = v4Up;
		a_m4Transform[2] = v4Forward;
	}
	else
	{
		sbMouse2ButtonDown = false;
	}
}

auto Utilities::GetRandomPosition(glm::vec3 a_min, glm::vec3 a_max) -> glm::vec3
{
	double x = GetRandomDouble(a_min.x, a_max.x);
	double y = GetRandomDouble(a_min.y, a_max.y);
	double z = GetRandomDouble(a_min.z, a_max.z);

	return {x, y, z};
}

auto Utilities::GetRandomDouble(double a_min, double a_max) -> double
{
	return (a_max - a_min) * (static_cast<double>(rand()) / static_cast<double>(RAND_MAX)) + a_min;
}

auto Utilities::GetRandomRotation() -> glm::quat
{
	//"Choosing a Point on the surface of a Sphere" - George Marsaglia - https://projecteuclid.org/journals/annals-of-mathematical-statistics/volume-43/issue-2/Choosing-a-Point-from-the-Surface-of-a-Sphere/10.1214/aoms/1177692644.full
	double u;
	double v;
	double w;
	double x;
	double y;
	double z;
	do
	{
		x = glm::linearRand(-1.0, 1.0);
		y = glm::linearRand(-1.0, 1.0);
		z = x * x + y * y;
	}
	while (z > 1); //generate  xyz
	do
	{
		u = glm::linearRand(-1.0, 1.0);
		v = glm::linearRand(-1.0, 1.0);
		w = u * u + v * v;
	}
	while (w > 1); // generate uvw
	double s = glm::sqrt((1 - z) / w); //calculate s 
	return {float(x), float(y), float(s * u), float(s * v)}; //return random quat, its already normalized as all values are between 0 and 1
}
