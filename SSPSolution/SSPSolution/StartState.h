#ifndef SSPAPPLICATION_GAMESTATES_STARTSTATE_H
#define SSPAPPLICATION_GAMESTATES_STARTSTATE_H
#include "GameState.h"
#include "MenuState.h"
#include "../GraphicsDLL/GraphicsComponent.h"

class StartState :
	public GameState
{
private:
	UIComponent* m_gamelogoUIComp;
	TextComponent* m_spaceTextComp;

	ComponentHandler* m_cHandlerPtr;
	Camera* m_cameraRef;

	DirectX::XMVECTOR m_lockTarget;
public:
	StartState();
	virtual ~StartState();

	int ShutDown();
	int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler, Camera* cameraRef);
	int Update(float dt, InputHandler * inputHandler);

	int EnterState();
	int LeaveState();

	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };
private:
};

#endif