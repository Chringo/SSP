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
		UIComponent* m_uiComp;
		TextComponent* m_textComp;

		void SetHovered(bool hovered)
		{
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
	const static int m_NR_OF_MENU_ITEMS = 2;
	MenuButton m_menuButtons[m_NR_OF_MENU_ITEMS];
	ComponentHandler* m_cHandlerPtr;
	Camera* m_cameraRef;
	DirectX::XMVECTOR m_lockTarget;
public:
	MenuState();
	~MenuState();

	int ShutDown();
	int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler, Camera* cameraRef);
	int Update(float dt, InputHandler * inputHandler);
private:
};

#endif