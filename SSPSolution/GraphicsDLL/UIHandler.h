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
	Cog = 3,
	Button = 4,
	Crosshair = 5,
	CrosshairAim = 6,
	Keymaps = 7,
	CombinedFrame = 8,
	Level0 = 9,
	Level1 = 10,
	Level2 = 11,
	Level3 = 12,
	Level4 = 13,
	Level5 = 14,
	LevelFrame = 15,
	Header = 16
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
