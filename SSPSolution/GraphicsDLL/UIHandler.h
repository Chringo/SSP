#ifndef GRAPHICSDLL_UI_UIHANDLER_H
#define GRAPHICSDLL_UI_UIHANDLER_H

#include "GraphicsComponent.h"
#include <vector>
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "WICTextureLoader.h"

enum Textures
{
	Gamelogo = 0,
	Background = 1,
	Chars = 2,
	Frame = 3,
	Header = 4,
	Cog = 5,
	BStartGame = 6,
	BOptions = 7,
	BExit = 8,
	BHost = 9,
	BJoin = 10,
	BIP = 11,
	BBack = 12,
	Level0 = 13,
	Level1 = 14,
	Level2 = 15,
	Level3 = 16,
	Level4 = 17,
	Level5 = 18,
	Crosshair = 19,
	CrosshairAim = 20
};

class UIHandler
{

private:
	std::vector<UIComponent*> m_UIComponents;
	std::vector<TextComponent*> m_textComponents;
	unsigned int m_nrOfUIComponents;
	unsigned int m_maxUIComponents;
	unsigned int m_nrOfTextComponents;
	unsigned int m_maxTextComponents;
	DirectX::SpriteBatch* m_spriteBatch;
	DirectX::SpriteFont* m_spriteFont;
	std::vector<ID3D11ShaderResourceView*> m_textures;
	size_t m_nrOfTextures;
	ID3D11BlendState* m_blendState;

public:
	UIHandler();
	~UIHandler();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void DrawUI();
	void Shutdown();

	UIComponent* GetNextUIComponent();
	int RemoveUIComponent(UIComponent* ptr);
	int RemoveLastUIComponent();
	TextComponent* GetNextTextComponent();
	void UpdateUIComponentsclicked(DirectX::XMFLOAT2 mousePos);
	void UpdateUIComponentsclicked(DirectX::XMFLOAT2 mousePos, DirectX::XMFLOAT2 windowSize);
};

#endif
