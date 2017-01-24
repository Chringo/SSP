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
	result = GameState::InitializeBase(gsh, cHandler, cameraRef);

	this->m_cHandlerPtr = cHandler;
	this->m_cameraRef = cameraRef;

	//Workaround for camera trying to access nullptr
	this->m_lockTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR targetOffset = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	float distance = 4.0f;
	this->m_cameraRef->SetCameraPivot(&this->m_lockTarget, targetOffset, distance);

	this->m_greetingTextComp = cHandler->GetTextComponent();
	this->m_greetingTextComp->active = 1;
	this->m_greetingTextComp->position = DirectX::XMFLOAT2(100.f, 250.f);
	this->m_greetingTextComp->scale = DirectX::XMFLOAT2(1.5f, 1.5f);
	this->m_greetingTextComp->text = L"Earl Grave's Prison of Puzzles";

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
			this->m_greetingTextComp->active = 0;
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
