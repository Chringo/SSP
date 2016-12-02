#ifndef SSPAPPLICATION_CORE_SYSTEM_H
#define SSPAPPLICATION_CORE_SYSTEM_H
#include <SDL.h>
#include <SDL_syswm.h>
#include <iostream>
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../GraphicsDLL/Camera.h"
#include "InputHandler.h"
#include "../physicsDLL/physicsDLL/PhysicsHandler.h"
#include "../NetworkDLL/NetworkModule.h"
#pragma comment (lib, "../Debug/PhysicsDLL")
#pragma comment (lib, "../Debug/NetworkDLL")

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

	Camera* m_camera;
	//These are the subsystems
	GraphicsHandler* m_graphicsHandler;
	InputHandler* m_inputHandler;
	//this is a physicsHandler
	PhysicsHandler m_physicsHandler;
	NetworkModule m_networkModule;
	char* m_ip = "192.168.1.141";	//Tobias NUC Specific local ip

public:
	System();
	~System();
	int Shutdown();

	int Initialize();
	//Do not place things here without talking to the system designers. Place any update method in the System::Update(float dt) method
	int Run();
	int Update(float deltaTime);

private:
	int HandleEvents();
	int FullscreenToggle();
};

#endif