#include "StartState.h"
#include "GameStateHandler.h"


StartState::StartState()
{
}


StartState::~StartState()
{
}

int StartState::ShutDown()
{
	int result = 1;
	return result;
}

int StartState::Initialize(GameStateHandler * gsh, ComponentHandler * cHandler, Camera * cameraRef)
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

	this->m_gamelogoUIComp = cHandler->GetUIComponent();
	this->m_gamelogoUIComp->active = 1;
	this->m_gamelogoUIComp->spriteID = Textures::Gamelogo;
	this->m_gamelogoUIComp->position = DirectX::XMFLOAT2(115.f, 0.f);
	this->m_gamelogoUIComp->size = DirectX::XMFLOAT2(800.f, 400.f);

	this->m_spaceTextComp = cHandler->GetTextComponent();
	this->m_spaceTextComp->active = 1;
	this->m_spaceTextComp->position = DirectX::XMFLOAT2(320.f, 550.f);
	this->m_spaceTextComp->text = L"Press SPACE to continue...";

	return result;
}

int StartState::Update(float dt, InputHandler * inputHandler)
{
	int result = 1;

	if (inputHandler->IsKeyPressed(SDL_SCANCODE_SPACE))
	{
		MenuState* menuState = new MenuState();
		result = menuState->Initialize(this->m_gsh, this->m_cHandlerPtr, this->m_cameraRef);

		if (result > 0)
		{
			//Push it to the gamestate stack/vector
			this->m_gsh->PushStateToStack(menuState);
			this->m_gamelogoUIComp->active = 0;
			this->m_spaceTextComp->active = 0;
		}
		else
		{
			//Delete it
			delete menuState;
			menuState = nullptr;
		}
	}

	return result;
}

int StartState::EnterState()
{
	return 0;
}

int StartState::LeaveState()
{
	return 0;
}
