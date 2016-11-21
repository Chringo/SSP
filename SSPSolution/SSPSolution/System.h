#ifndef SSPAPPLICATION_CORE_SYSTEM_H
#define SSPAPPLICATION_CORE_SYSTEM_H
#include <SDL.h>
#include <iostream>
#include <SDL_syswm.h>
#include "InputHandler.h"
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
class System
{
private:
	bool m_fullscreen;
	bool m_running;
	//The glorious window handle for the sdl window
	HWND m_hwnd;
	HINSTANCE m_hinstance;
	LPCWSTR m_applicationName;
	//This is the window we render to
	SDL_Window* m_window;
	//These are the components
	InputHandler* m_inputHandler;
public:
	System();
	~System();
	int Shutdown();

	int Initialize();
	int Run();

private:
	int HandleEvents();
	int FullscreenToggle();
};

#endif