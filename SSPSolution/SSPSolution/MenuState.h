#ifndef SSPAPPLICATION_GAMESTATES_MENUSTATE_H
#define SSPAPPLICATION_GAMESTATES_MENUSTATE_H
#include "GameState.h"
#include "LevelSelectState.h"
#include "../GraphicsDLL/GraphicsComponent.h"
class MenuState :
	public GameState
{
private:
	struct MenuButton {
		bool m_hovered;
		UIComponent* m_uiComp;
		TextComponent* m_textComp;

		void SetHovered(bool hovered)
		{
			m_hovered = hovered;
			if (hovered)
			{
				m_uiComp->scale = 1.25f;
				m_textComp->scale = DirectX::XMFLOAT2(1.25f, 1.25f);
			}
			else
			{
				m_uiComp->scale = 1.f;
				m_textComp->scale = DirectX::XMFLOAT2(1.f, 1.f);
			}
		}
		void SetActive(bool active)
		{
			if (active)
			{
				m_uiComp->active = 1;
				m_textComp->active = 1;
			}
			else
			{
				m_uiComp->active = 0;
				m_textComp->active = 0;
			}
		}
	};
	struct TextBox {
		UIComponent* m_uiComp;
		TextComponent* m_textComp;
		bool m_focused;
		bool firstChar = true;

		void SetActive(bool active)
		{
			if (active)
			{
				m_uiComp->active = 1;
				m_textComp->active = 1;
			}
			else
			{
				m_uiComp->scale = 1.f;
				m_uiComp->active = 0;
				m_textComp->active = 0;
				m_focused = false;
			}
		}
		void SetFocused(bool focused)
		{
			if (focused)
			{
				m_uiComp->scale = 1.25f;
				m_focused = true;
			}
			else
			{
				m_uiComp->scale = 1.f;
				m_focused = false;
			}
		}
		void AddChar(std::wstring character)
		{
			if (firstChar)
			{
				m_textComp->text.clear();
				this->firstChar = false;
			}
			m_textComp->text.append(character);
		}
		void RemoveChar()
		{
			if (!m_textComp->text.empty() && !this->firstChar)
			{
				m_textComp->text.pop_back();
				if (m_textComp->text.empty())
				{
					this->firstChar = true;
					m_textComp->text = L"Enter IP...";
				}
			}
		}
	};

	const static int m_NR_OF_MAIN_MENU_ITEMS = 3;
	const static int m_NR_OF_OPTIONS_MENU_ITEMS = 2;
	const static int m_NR_OF_MENU_ITEMS = m_NR_OF_MAIN_MENU_ITEMS + m_NR_OF_OPTIONS_MENU_ITEMS;
	unsigned int m_menuState; //0: Main menu, 1: Options Menu
	unsigned int m_markedItem;
	MenuButton m_menuButtons[m_NR_OF_MENU_ITEMS];
	TextBox m_ipTextBox;
	ComponentHandler* m_cHandlerPtr;
	Camera* m_cameraRef;
	DirectX::XMVECTOR m_lockTarget;
public:
	MenuState();
	~MenuState();

	int ShutDown();
	int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler, Camera* cameraRef);
	int Update(float dt, InputHandler * inputHandler);

	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };
private:
};

#endif