#ifndef SSPAPPLICATION_GAMESTATES_MENUSTATE_H
#define SSPAPPLICATION_GAMESTATES_MENUSTATE_H
#include "GameState.h"
#include "LevelSelectState.h"
#include "../GraphicsDLL/GraphicsComponent.h"

#define HOST_DISABLE
#define NR_OF_LEVELS 6

const float TIMEOUT_TIME = 200;

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
				//m_uiComp->scale = 1.0f;
				m_textComp->scale = DirectX::XMFLOAT2(0.7f, 0.6f);
			}
			else
			{
				//m_uiComp->scale = 0.9f;
				m_textComp->scale = DirectX::XMFLOAT2(0.6f, 0.6f);
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
				m_uiComp->active = 0;
				m_textComp->active = 0;
				m_focused = false;
			}
		}
		void SetFocused(bool focused)
		{
			if (focused)
			{
				m_textComp->scale = DirectX::XMFLOAT2(0.6f, 0.5f);
				m_focused = true;
			}
			else
			{
				m_textComp->scale = DirectX::XMFLOAT2(0.5f, 0.5f);
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
			if(m_textComp->text.size() < 15)
			{ 
				m_textComp->text.append(character);
			}
		}
		void RemoveChar()
		{
			if (!m_textComp->text.empty() && !this->firstChar)
			{
				m_textComp->text.pop_back();
				if (m_textComp->text.empty())
				{
					this->firstChar = true;
				}
			}
		}
	};

	unsigned int m_menuState; //0: Main Menu, 1: Options Menu, 2: Start Menu, 3: Level Select
	unsigned int m_markedItem;
	UIComponent* m_menuBG;
	UIComponent* m_menuFrame;
	UIComponent* m_levelFrame;
	std::vector<UIComponent*> m_menuCogs;
	std::vector<MenuButton> m_mainMenuButtons;
	std::vector<MenuButton> m_optionsMenuButtons;
	bool inFullscreen;
	//UIComponent* m_keymaps;
	std::vector<MenuButton> m_startMenuButtons;
	TextBox m_ipTextBox;
	std::vector<MenuButton> m_hostMenuButtons;
	int m_levelToHost;
	Camera* m_cameraRef;
	DirectX::XMVECTOR m_lockTarget;

	//music variables
	irrklang::ISound* m_Menu_Music;
	irrklang::ISound* m_Level_Music;
	
	//Network related functions and variables
	bool isHosting;
	bool isJoining;
	float timeoutTime;
	bool sentSyncPacket;

	void Hosting(float dt, InputHandler* inputHandler);
	void Joining(InputHandler* inputHandler);
	void setMusic(Sounds2D which);

public:
	MenuState();
	~MenuState();

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