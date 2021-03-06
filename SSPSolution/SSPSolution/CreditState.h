#ifndef SSPAPPLICATION_GAMESTATES_CREDITSTATE_H
#define SSPAPPLICATION_GAMESTATES_CREDITSTATE_H
#include "GameState.h"
#include "MenuState.h"
#include "../GraphicsDLL/GraphicsComponent.h"

class CreditState :
	public GameState
{
private:
	UIComponent* m_gamelogoUIComp;
	UIComponent* m_backgroundUIComp;
	TextComponent* m_gzTextComp;
	TextComponent* m_creditHeaderTextComp;
	TextComponent* m_creditTextComp;

	ComponentHandler* m_cHandlerPtr;
	Camera* m_cameraRef;

	DirectX::XMVECTOR m_lockTarget;
public:
	CreditState();
	virtual ~CreditState();

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