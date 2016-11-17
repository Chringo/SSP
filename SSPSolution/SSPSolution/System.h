#ifndef SSPAPPLICATION_CORE_SYSTEM_H
#define SSPAPPLICATION_CORE_SYSTEM_H
#include <SDL.h>
#include <iostream>
#include <SDL_syswm.h>
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;
class System
{
private:
	bool m_fullscreen;
	bool m_running;
	//The glorious window handle for the sdl window
	HWND m_hwnd;
	//This is the window we render to
	SDL_Window* m_window;
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