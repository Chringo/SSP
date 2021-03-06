#ifndef SSPAPPLICATION_CORE_SYSTEM_H
#define SSPAPPLICATION_CORE_SYSTEM_H
#include <SDL.h>
#include <SDL_syswm.h>
#include <omp.h>
#include <iostream>
#include "InputHandler.h"
#include "GameStateHandler.h"
#include "DebugHandler.h"
#include "Progression.h"
//Project DLLs
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../GraphicsDLL/Camera.h"
#include "../GraphicsDLL/AnimationHandler.h"
#include "../physicsDLL/PhysicsHandler.h"
#include "../AIDLL/AIHandler.h"
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
	GameStateHandler m_gsh;

	Camera* m_camera;
	//This is the inputhandler which stores the state of the inputs
	InputHandler* m_inputHandler;
	//These are the subsystems
	GraphicsHandler* m_graphicsHandler;
	AnimationHandler* m_AnimationHandler; //Handles all animations for entities. 
	PhysicsHandler m_physicsHandler; //this is a physicsHandler
	AIHandler m_AIHandler;
	//This is the component handler which acts as an interface between the subsystems and the states
	ComponentHandler m_componentHandler;
public:
	System();
	~System();
	int Shutdown();

	int Initialize(std::string path = "");
	//Do not place things here without talking to the system designers. Place any update method in the System::Update(float dt) method
	int Run();
	int Update(float deltaTime);

	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };

private:
	int HandleEvents();
	int FullscreenToggle();

	//temporary functions
	void LockCameraToPlayer(float translateCameraX, float translateCameraY, float translateCameraZ);
};

#endif