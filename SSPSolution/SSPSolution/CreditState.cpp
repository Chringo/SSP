#include "CreditState.h"
#include "GameStateHandler.h"


CreditState::CreditState()
{
}


CreditState::~CreditState()
{
}

int CreditState::ShutDown()
{
	int result = 1;
	this->m_cHandlerPtr->RemoveLastUIComponent();
	this->m_cHandlerPtr->RemoveLastUIComponent();
	return result;
}

int CreditState::Initialize(GameStateHandler * gsh, ComponentHandler * cHandler, Camera * cameraRef)
{
	int result = 0;
	result = GameState::InitializeBase(gsh, cHandler, cameraRef, false);

	this->m_cHandlerPtr = cHandler;
	this->m_cameraRef = cameraRef;

	//Workaround for camera trying to access nullptr
	this->m_lockTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR targetOffset = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	float distance = 4.0f;
	this->m_cameraRef->SetCameraPivot(&this->m_lockTarget, targetOffset, distance);

	this->m_backgroundUIComp = cHandler->GetUIComponent();
	this->m_backgroundUIComp->active = 1;
	this->m_backgroundUIComp->spriteID = Textures::Background;
	this->m_backgroundUIComp->position = DirectX::XMFLOAT2(0.f, 0.f);
	this->m_backgroundUIComp->scale = 0.66666f;
	this->m_backgroundUIComp->size = DirectX::XMFLOAT2(800.f, 400.f);

	this->m_gamelogoUIComp = cHandler->GetUIComponent();
	this->m_gamelogoUIComp->active = 1;
	this->m_gamelogoUIComp->spriteID = Textures::Header;
	this->m_gamelogoUIComp->position = DirectX::XMFLOAT2(470.f, 30.5f);
	this->m_gamelogoUIComp->scale = 0.66666f;
	this->m_gamelogoUIComp->size = DirectX::XMFLOAT2(400.f, 400.f);

	this->m_spaceTextComp = cHandler->GetTextComponent();
	this->m_spaceTextComp->active = 1;
	this->m_spaceTextComp->scale = DirectX::XMFLOAT2(0.5f, 0.5f);
	this->m_spaceTextComp->position = DirectX::XMFLOAT2(320.f, 550.f);
	this->m_spaceTextComp->text = L"Press SPACE to continue...";

	return result;
}

int CreditState::Update(float dt, InputHandler * inputHandler)
{
	int result = 1;

	if (inputHandler->IsKeyPressed(SDL_SCANCODE_SPACE) || inputHandler->IsKeyPressed(SDL_SCANCODE_ESCAPE))
	{
		this->m_gsh->PopStateFromStack();
	}

	return result;
}

int CreditState::EnterState()
{
	return 0;
}

int CreditState::LeaveState()
{
	return 0;
}
