#ifndef SSPAPPLICATION_GAMESTATES_GAMESTATE_H
#define SSPAPPLICATION_GAMESTATES_GAMESTATE_H
#include "../NetworkDLL/NetworkModule.h"
#pragma comment (lib, "../Debug/NetworkDLL")
#include "InputHandler.h"
#include "ComponentHandler.h"
#include "../GraphicsDLL/Camera.h"
#include "Progression.h"

class GameStateHandler;
class GameState
{
private:	//Variables	

protected:
	GameStateHandler* m_gsh;
	ComponentHandler* m_cHandler;
	Camera* m_cameraRef;	
	char* m_ip = "192.168.1.25";	//Tobias NUC Specific local ip


	int InitializeBase(GameStateHandler* gsh, ComponentHandler* cHandler, Camera* cameraRef);
public:
	static NetworkModule* m_networkModule;	// Is public so we can accses it from GameStateHandler for Shutdown

	GameState();
	virtual ~GameState();

	virtual int ShutDown() = 0;
	//Returns 1 for success and 0 for failure
	virtual int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler, Camera* cameraRef) = 0;

	virtual int Update(float dt, InputHandler * inputHandler) = 0;
private:	//Helper functions
};

#endif