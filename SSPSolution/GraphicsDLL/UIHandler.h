#ifndef GRAPHICSDLL_UI_UIHANDLER_H
#define GRAPHICSDLL_UI_UIHANDLER_H

#include "GraphicsComponent.h"
#include <vector>
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "WICTextureLoader.h"

class UIHandler
{

private:
	std::vector<UIComponent*> m_UIComponents;
	std::vector<TextComponent> m_textComponents;
	unsigned int m_nrOfComponents;
	unsigned int m_maxComponents;
	DirectX::SpriteBatch* m_spriteBatch;
	DirectX::SpriteFont* m_spriteFont;
	ID3D11ShaderResourceView* m_texture;

public:
	UIHandler();
	~UIHandler();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void DrawUI();
	void Shutdown();

	UIComponent* GetNextUIComponent();
	void UpdateUIComponentsclicked(DirectX::XMFLOAT2 mousePos);
	void UpdateUIComponentsclicked(DirectX::XMFLOAT2 mousePos, DirectX::XMFLOAT2 windowSize);
};

#endif
