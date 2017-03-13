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
	this->m_cHandlerPtr->RemoveLastTextComponent();
	this->m_cHandlerPtr->RemoveLastTextComponent();
	this->m_cHandlerPtr->RemoveLastTextComponent();
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
	this->m_backgroundUIComp->position = DirectX::XMFLOAT2(0.0f, 0.0f);
	this->m_backgroundUIComp->scale = 0.66666f;
	this->m_backgroundUIComp->size = DirectX::XMFLOAT2(800.0f, 400.0f);

	this->m_gamelogoUIComp = cHandler->GetUIComponent();
	this->m_gamelogoUIComp->active = 1;
	this->m_gamelogoUIComp->spriteID = Textures::Header;
	this->m_gamelogoUIComp->position = DirectX::XMFLOAT2(470.0f, 30.5f);
	this->m_gamelogoUIComp->scale = 0.66666f;
	this->m_gamelogoUIComp->size = DirectX::XMFLOAT2(400.0f, 400.0f);

	this->m_gzTextComp = cHandler->GetTextComponent();
	this->m_gzTextComp->active = 1;
	this->m_gzTextComp->scale = DirectX::XMFLOAT2(0.5f, 0.5f);
	this->m_gzTextComp->position = DirectX::XMFLOAT2(80.0f, 220.0f);
	this->m_gzTextComp->text = L"Congratulations on your seemingly feeble attempts at escaping which, surprisingly to all,\nmanaged to get you out of my, Earl Grave's, marvelous prison complex.\nOn everyones behalf, I congratulate you. Well done.";

	float creditTextScale = 1.0f / 2.0f;

	this->m_creditHeaderTextComp = cHandler->GetTextComponent();
	this->m_creditHeaderTextComp->active = 1;
	this->m_creditHeaderTextComp->scale = DirectX::XMFLOAT2(creditTextScale * 1.1f, creditTextScale * 1.1f);
	this->m_creditHeaderTextComp->position = DirectX::XMFLOAT2(80.0f, 470.0f);
	this->m_creditHeaderTextComp->text = L"Developers:";

	this->m_creditTextComp = cHandler->GetTextComponent();
	this->m_creditTextComp->active = 1;
	this->m_creditTextComp->scale = DirectX::XMFLOAT2(creditTextScale, creditTextScale);
	this->m_creditTextComp->position = DirectX::XMFLOAT2(80.0f, 510.0f);
	this->m_creditTextComp->text = L"Christoffer Banheden      Marc Andersson      John Augustsson      Kim Arvola Bjelkesten\nMartin Clementson         Oscar Engborg       Johan Ganeteg        Sebastian Hallin\nErik Petterson            Tobias Kling        Axel Persson         David Wigelius\nSebastian Lundgren";
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
