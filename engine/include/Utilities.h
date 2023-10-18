// File: Utilities.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/04/11
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Declarations for random utilities that didn't have a home anywhere else

#ifndef __UTILITIES_H__
#define __UTILITIES_H__
#include <string>
#include <glm.hpp>

class Utilities
{
public:

	//utilities for timing
	static void  TimerReset();
	static float TimerTick();
	static float getDeltaTime();
	static float getTotalTime();

	//utilites for file loading
	static char* FileToBuffer(const std::string& a_strFilePath);
	static char* FileToBuffer(const std::string& a_strFilePath, std::streamsize& a_rFileSize);

	//camera movement utiltiy
	static void FreeMovement(glm::mat4& a_m4Transform, float a_fDeltaTime = 1.0f / 60.0f, float a_fSpeed = 2.0f, const glm::vec3& a_v3Up = { 0,1,0 });

	//random utilities
	static glm::vec3 GetRandomPosition(glm::vec3 a_min, glm::vec3 a_max);
	static double GetRandomDouble(double a_min, double a_max);
	static glm::quat GetRandomRotation();
};
#endif // !__UTILITIES_H__