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
	RenderWindow* modelApp = new RenderWindow();
	modelApp->Run("Model Loader", 1280, 720, false);
	delete modelApp;
	return 0;
}