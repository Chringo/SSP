#ifndef SSPAPPLICATION_GAMESTATES_GAMESTATE_H
#define SSPAPPLICATION_GAMESTATES_GAMESTATE_H
#include "../NetworkDLL/NetworkModule.h"
#pragma comment (lib, "../Debug/NetworkDLL")
#include "InputHandler.h"
#include "ComponentHandler.h"
#include "../GraphicsDLL/Camera.h"

class GameStateHandler;
class GameState
{
private:	//Variables	

protected:
	GameStateHandler* m_gsh;
	ComponentHandler* m_cHandler;
	Camera* m_cameraRef;
	static NetworkModule* m_networkModule;
	char* m_ip = "192.168.1.141";	//Tobias NUC Specific local ip


	int InitializeBase(GameStateHandler* gsh, ComponentHandler* cHandler, Camera* cameraRef);
public:
	GameState();
	virtual ~GameState();

	virtual int ShutDown() = 0;
	//Returns 1 for success and 0 for failure
	virtual int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler, Camera* cameraRef) = 0;

	virtual int Update(float dt, InputHandler * inputHandler) = 0;
private:	//Helper functions
};

#endif