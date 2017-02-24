#ifndef GRAPHICSDLL_UI_UIHANDLER_H
#define GRAPHICSDLL_UI_UIHANDLER_H

#include "GraphicsComponent.h"
#include <vector>
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "WICTextureLoader.h"

enum Textures
{
	Cat = 0,
	Gamelogo = 1,
	Keymaps = 2,
	MenuBG = 3,
	Button = 4,
	Crosshair = 5,
	CrosshairAim = 6
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
	TextComponent* GetNextTextComponent();
	void UpdateUIComponentsclicked(DirectX::XMFLOAT2 mousePos);
	void UpdateUIComponentsclicked(DirectX::XMFLOAT2 mousePos, DirectX::XMFLOAT2 windowSize);
};

#endif
