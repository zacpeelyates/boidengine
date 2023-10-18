// File: Application.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/01/24
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Declarations for running GLFW Window

#ifndef APPLICATION_H
#define APPLICATION_H
struct GLFWwindow;
class Application
{
public:
	Application() : m_window(nullptr), m_windowWidth(0), m_windowHeight(0), m_running(false) {}
	virtual ~Application() {}
	bool Create(const char* ap_appName, unsigned int a_winWidth, unsigned int a_winHeight, bool fullscreen);
	void Run(const char* ap_appName, unsigned int a_winWidth, unsigned int a_winHeight, bool fullscreen);
	void Quit() { m_running = false; };

protected:
	virtual bool onCreate() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;
	virtual void Destroy() = 0;

	GLFWwindow* m_window;
	unsigned int m_windowWidth;
	unsigned int m_windowHeight;
	bool m_running;

};
#endif // APPLICATION_H
