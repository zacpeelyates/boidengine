// File: main.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/01/24
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Main Entry Point of Application

#include "RenderWindow.h"

auto main(int /*argc*/, char* /*argv*/[]) -> int
{
	//Main entry point of application
	auto* renderWindow = new RenderWindow();
	renderWindow->Run("CT5037 Boid Engine", 1280, 720, false);
	delete renderWindow;
	return 0;
}
