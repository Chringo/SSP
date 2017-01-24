#include "MenuState.h"
#include "GameStateHandler.h"


MenuState::MenuState()
{
}


MenuState::~MenuState()
{
}

int MenuState::ShutDown()
{
	int result = 1;
	return result;
}

int MenuState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler, Camera* cameraRef)
{
	int result = 0;

	result = GameState::InitializeBase(gsh, cHandler, cameraRef);

	this->m_cHandlerPtr = cHandler;
	this->m_cameraRef = cameraRef;
	
	this->m_lockTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR targetOffset = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	float distance = 4.0f;
	this->m_cameraRef->SetCameraPivot(&this->m_lockTarget, targetOffset, distance);

	for (size_t i = 0; i < m_NR_OF_MENU_ITEMS; i++)
	{
		this->m_menuButtons[i].m_uiComp = cHandler->GetUIComponent();
		this->m_menuButtons[i].m_uiComp->active = 1;
		this->m_menuButtons[i].m_uiComp->position = DirectX::XMFLOAT2(100.f, 200.f + (i * 150.f));
		this->m_menuButtons[i].m_uiComp->size = DirectX::XMFLOAT2(400.f, 100.f);
		this->m_menuButtons[i].m_textComp = cHandler->GetTextComponent();
		this->m_menuButtons[i].m_textComp->active = 1;
		this->m_menuButtons[i].m_textComp->position = DirectX::XMFLOAT2(125.f, 220.f + (i * 150.f));
	}

	this->m_menuButtons[0].m_textComp->text = L"Start Game";
	this->m_menuButtons[1].m_textComp->text = L"Quit Game";


	return result;
}

int MenuState::Update(float dt, InputHandler * inputHandler)
{
	int result = 1;

	DirectX::XMFLOAT2 mousePos = inputHandler->GetMousePos();

	if (inputHandler->IsMouseKeyReleased(SDL_BUTTON_LEFT))
	{
		for (size_t i = 0; i < m_NR_OF_MENU_ITEMS; i++)
		{
			this->m_menuButtons[i].m_uiComp->UpdateClicked(mousePos);
		}
	}

	for (size_t i = 0; i < m_NR_OF_MENU_ITEMS; i++)
	{
		this->m_menuButtons[i].m_uiComp->UpdateHover(mousePos);
		if (this->m_menuButtons[i].m_uiComp->isHovered)
		{
			this->m_menuButtons[i].SetHovered(true);
		}
		else
		{
			this->m_menuButtons[i].SetHovered(false);
		}
	}

	if (this->m_menuButtons[0].m_uiComp->CheckClicked())
	{
		//Start game was clicked

		//Create, Initialize and push a LevelSelectState
		LevelSelectState* levelSelect = new LevelSelectState();
		result = levelSelect->Initialize(this->m_gsh, this->m_cHandlerPtr, this->m_cameraRef);

		//If the initialization was successful
		if (result > 0)
		{
			//Push it to the gamestate stack/vector
			this->m_gsh->PushStateToStack(levelSelect);
			
			
			levelSelect->LoadLevel(std::string("../ResourceLib/AssetFiles/TestingLevel.level"));
		}
		else
		{
			//Delete it
			delete levelSelect;
			levelSelect = nullptr;
		}

		for (size_t i = 0; i < m_NR_OF_MENU_ITEMS; i++)
		{
			this->m_menuButtons[i].SetActive(false);
		}
	}
	else if (this->m_menuButtons[1].m_uiComp->CheckClicked())
	{
		//Quit game was clicked
		result = 0;
	}

	return result;
}
