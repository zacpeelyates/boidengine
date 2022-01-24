//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: main.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 21/01/22
// Last Edited:  21/01/22
// Brief: Main entry point of application. 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "RenderWindow.h"
int main(int argc, char* argv[])
{
	//Main entry point of application
	RenderWindow* renderWindow = new RenderWindow();
	renderWindow->Run("CT5037 Boid Engine", 1280, 720, false);
	delete renderWindow;
	return 0;
}