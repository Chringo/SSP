#include "MenuState.h"



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

int MenuState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler)
{
	int result = 0;
	result = GameState::InitializeBase(gsh, cHandler);

	for (size_t i = 0; i < m_NR_OF_MENU_ITEMS; i++)
	{
		this->m_uiComps[i] = cHandler->GetUIComponent();
		this->m_uiComps[i]->position = DirectX::XMFLOAT2(100.f, 200.f + (i * 100.f));
		this->m_uiComps[i]->size = DirectX::XMFLOAT2(100.f, 75.f);
		this->m_textComps[i] = cHandler->GetTextComponent();
		this->m_textComps[i]->position = DirectX::XMFLOAT2(125.f, 220.f + (i * 100.f));
	}

	this->m_textComps[0]->text = L"Start Game";
	this->m_textComps[1]->text = L"Quit Game";

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
			this->m_uiComps[i]->UpdateClicked(mousePos);
		}
	}

	for (size_t i = 0; i < m_NR_OF_MENU_ITEMS; i++)
	{
		this->m_uiComps[i]->UpdateHover(mousePos);
		if (this->m_uiComps[i]->isHovered)
		{
			this->m_uiComps[i]->scale = 1.25f;
			this->m_textComps[i]->scale = DirectX::XMFLOAT2(1.25f, 1.25f);
		}
		else
		{
			this->m_uiComps[i]->scale = 1.f;
			this->m_textComps[i]->scale = DirectX::XMFLOAT2(1.f, 1.f);
		}
	}

	if (this->m_uiComps[0]->CheckClicked())
	{
		//Start game was clicked
	}
	else if (this->m_uiComps[1]->CheckClicked())
	{
		//Quit game was clicked
		result = 0;
	}

	return result;
}
