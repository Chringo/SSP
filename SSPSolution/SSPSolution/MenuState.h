#ifndef SSPAPPLICATION_GAMESTATES_MENUSTATE_H
#define SSPAPPLICATION_GAMESTATES_MENUSTATE_H
#include "GameState.h"
#include "../GraphicsDLL/GraphicsComponent.h"
class MenuState :
	public GameState
{
private:
	const static int m_NR_OF_MENU_ITEMS = 2;
	UIComponent* m_uiComps[NR_OF_MENU_ITEMS];
	TextComponent* m_textComps[NR_OF_MENU_ITEMS];
public:
	MenuState();
	~MenuState();

	int ShutDown();
	int Initialize(GameStateHandler* gsh, ComponentHandler* cHandler);
	int Update(float dt, InputHandler * inputHandler);
private:
};

#endif