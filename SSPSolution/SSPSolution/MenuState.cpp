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

	for (size_t i = 0; i < m_NR_OF_MAIN_MENU_ITEMS; i++)
	{
		this->m_menuButtons[i].m_uiComp = cHandler->GetUIComponent();
		this->m_menuButtons[i].m_uiComp->active = 1;
		this->m_menuButtons[i].m_uiComp->position = DirectX::XMFLOAT2(100.f, 200.f + (i * 150.f));
		this->m_menuButtons[i].m_uiComp->size = DirectX::XMFLOAT2(400.f, 100.f);
		this->m_menuButtons[i].m_textComp = cHandler->GetTextComponent();
		this->m_menuButtons[i].m_textComp->active = 1;
		this->m_menuButtons[i].m_textComp->position = DirectX::XMFLOAT2(125.f, 220.f + (i * 150.f));
	}
	for (size_t i = m_NR_OF_MAIN_MENU_ITEMS; i < m_NR_OF_MENU_ITEMS; i++)
	{
		int newI = i - m_NR_OF_MAIN_MENU_ITEMS;
		this->m_menuButtons[i].m_uiComp = cHandler->GetUIComponent();
		this->m_menuButtons[i].m_uiComp->active = 0;
		this->m_menuButtons[i].m_uiComp->position = DirectX::XMFLOAT2(100.f, 200.f + (newI * 150.f));
		this->m_menuButtons[i].m_uiComp->size = DirectX::XMFLOAT2(400.f, 100.f);
		this->m_menuButtons[i].m_textComp = cHandler->GetTextComponent();
		this->m_menuButtons[i].m_textComp->active = 0;
		this->m_menuButtons[i].m_textComp->position = DirectX::XMFLOAT2(125.f, 220.f + (newI * 150.f));
	}

	this->m_ipTextBox.m_uiComp = cHandler->GetUIComponent();
	this->m_ipTextBox.m_uiComp->active = 0;
	this->m_ipTextBox.m_uiComp->position = DirectX::XMFLOAT2(550.f, 200.f + (150.f));
	this->m_ipTextBox.m_uiComp->size = DirectX::XMFLOAT2(400.f, 100.f);
	this->m_ipTextBox.m_textComp = cHandler->GetTextComponent();
	this->m_ipTextBox.m_textComp->active = 0;
	this->m_ipTextBox.m_textComp->position = DirectX::XMFLOAT2(575.f, 220.f + (150.f));
	this->m_ipTextBox.m_textComp->text = L"Enter ip...";

	this->m_menuButtons[0].m_textComp->text = L"Start Game";
	this->m_menuButtons[2].m_textComp->text = L"Quit Game";
	this->m_menuButtons[1].m_textComp->text = L"Options";

	this->m_menuButtons[3].m_textComp->text = L"Toggle Fullscreen";
	this->m_menuButtons[4].m_textComp->text = L"Go Back";

	this->m_markedItem = 0;
	this->m_menuButtons[0].SetHovered(true);

	return result;
}

int MenuState::Update(float dt, InputHandler * inputHandler)
{
	int result = 1;

	DirectX::XMFLOAT2 mousePos = inputHandler->GetMousePos();

	switch (this->m_menuState)
	{
	case 0: //Main menu
		for (size_t i = 0; i < m_NR_OF_MAIN_MENU_ITEMS; i++)
		{
			this->m_menuButtons[i].m_uiComp->UpdateHover(mousePos);
			if (this->m_menuButtons[i].m_uiComp->isHovered)
			{
				this->m_menuButtons[i].SetHovered(true);
				this->m_markedItem = i;
			}
			else if (i != this->m_markedItem)
			{
				this->m_menuButtons[i].SetHovered(false);
			}
		}

		if (inputHandler->IsMouseKeyReleased(SDL_BUTTON_LEFT))
		{
			for (size_t i = 0; i < m_NR_OF_MAIN_MENU_ITEMS; i++)
			{
				this->m_menuButtons[i].m_uiComp->UpdateClicked(mousePos);
			}
		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_RETURN))
		{
			this->m_menuButtons[m_markedItem].m_uiComp->wasClicked = true;
		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_DOWN))
		{
			if (this->m_markedItem < this->m_NR_OF_MAIN_MENU_ITEMS - 1)
			{
				this->m_menuButtons[this->m_markedItem].SetHovered(false);
				this->m_markedItem++;
				this->m_menuButtons[this->m_markedItem].SetHovered(true);
			}
		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_UP))
		{
			if (this->m_markedItem > 0)
			{
				this->m_menuButtons[this->m_markedItem].SetHovered(false);
				this->m_markedItem--;
				this->m_menuButtons[this->m_markedItem].SetHovered(true);
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

			for (size_t i = 0; i < m_NR_OF_MAIN_MENU_ITEMS; i++)
			{
				this->m_menuButtons[i].SetActive(false);
			}
		}
		else if (this->m_menuButtons[1].m_uiComp->CheckClicked())
		{
			//Options was clicked
			this->m_menuButtons[this->m_markedItem].SetHovered(false);
			this->m_markedItem = this->m_NR_OF_MAIN_MENU_ITEMS;
			this->m_menuButtons[this->m_NR_OF_MAIN_MENU_ITEMS].SetHovered(true);
			this->m_menuState = 1;
			for (size_t i = 0; i < m_NR_OF_MAIN_MENU_ITEMS; i++)
			{
				this->m_menuButtons[i].SetActive(false);
			}
			for (size_t i = m_NR_OF_MAIN_MENU_ITEMS; i < m_NR_OF_MENU_ITEMS; i++)
			{
				this->m_menuButtons[i].SetActive(true);
			}
			this->m_ipTextBox.SetActive(true);
		}
		else if (this->m_menuButtons[2].m_uiComp->CheckClicked())
		{
			//Quit game was clicked
			result = 0;
		}
		break;

	case 1: //Options Menu
		for (size_t i = m_NR_OF_MAIN_MENU_ITEMS; i < m_NR_OF_OPTIONS_MENU_ITEMS; i++)
		{
			this->m_menuButtons[i].m_uiComp->UpdateHover(mousePos);
			if (this->m_menuButtons[i].m_uiComp->isHovered)
			{
				this->m_menuButtons[i].SetHovered(true);
				this->m_markedItem = i;
			}
			else if (i != this->m_markedItem)
			{
				this->m_menuButtons[i].SetHovered(false);
			}
		}

		if (inputHandler->IsMouseKeyReleased(SDL_BUTTON_LEFT))
		{
			for (size_t i = m_NR_OF_MAIN_MENU_ITEMS; i < m_NR_OF_OPTIONS_MENU_ITEMS; i++)
			{
				this->m_menuButtons[i].m_uiComp->UpdateClicked(mousePos);
			}
			this->m_ipTextBox.m_uiComp->UpdateClicked(mousePos);
		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_RETURN))
		{
			if (!this->m_ipTextBox.m_focused)
			{
				this->m_menuButtons[m_markedItem].m_uiComp->wasClicked = true;
			}
			else 
			{
				this->m_ipTextBox.SetFocused(false);
				this->m_markedItem--;
				this->m_menuButtons[this->m_markedItem].SetHovered(true);
			}
		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_DOWN))
		{
			if (this->m_markedItem < this->m_NR_OF_MENU_ITEMS)
			{
				this->m_menuButtons[this->m_markedItem].SetHovered(false);
				this->m_markedItem++;
				if (this->m_markedItem != this->m_NR_OF_MENU_ITEMS)
				{
					this->m_menuButtons[this->m_markedItem].SetHovered(true);
				}
				else
				{
					this->m_ipTextBox.SetFocused(true);
				}
			}
		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_UP))
		{
			if (this->m_markedItem > this->m_NR_OF_MAIN_MENU_ITEMS)
			{
				if (this->m_markedItem < this->m_NR_OF_MENU_ITEMS)
				{
					this->m_menuButtons[this->m_markedItem].SetHovered(false);
				}
				else
				{
					this->m_ipTextBox.SetFocused(false);
				}
				this->m_markedItem--;
				this->m_menuButtons[this->m_markedItem].SetHovered(true);
			}
		}

		if (this->m_menuButtons[3].m_uiComp->CheckClicked())
		{
			//Toggle fullscreen was clicked
			//Cheating by telling the system the user pressed F
			inputHandler->SetKeyState(SDL_SCANCODE_F, true); //Seems this does not reset, ever
		}
		else if (this->m_menuButtons[4].m_uiComp->CheckClicked())
		{
			//Return to main menu was clicked
			inputHandler->SetKeyState(SDL_SCANCODE_F, false); //Temporary cheat
			this->m_menuButtons[this->m_markedItem].SetHovered(false);
			this->m_markedItem = 0;
			this->m_menuButtons[0].SetHovered(true);
			this->m_menuState = 0;
			for (size_t i = 0; i < m_NR_OF_MAIN_MENU_ITEMS; i++)
			{
				this->m_menuButtons[i].SetActive(true);
			}
			for (size_t i = m_NR_OF_MAIN_MENU_ITEMS; i < m_NR_OF_MENU_ITEMS; i++)
			{
				this->m_menuButtons[i].SetActive(false);
			}
			this->m_ipTextBox.SetActive(false);
		}

		if (this->m_ipTextBox.m_focused)
		{
			if (inputHandler->IsKeyPressed(SDL_SCANCODE_1))
			{
				this->m_ipTextBox.m_textComp->text += L"1";
			}
		}

		break;
	default:
		break;
	}
	

	return result;
}
