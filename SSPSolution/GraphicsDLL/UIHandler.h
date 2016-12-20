#ifndef GRAPHICSDLL_UI_UIHANDLER_H
#define GRAPHICSDLL_UI_UIHANDLER_H

#include "GraphicsComponent.h"
#include <vector>

class UIHandler
{

private:
	std::vector<UIComponent*> m_UIComponents;
	unsigned int m_nrOfComponents;
	unsigned int m_maxComponents;

public:
	UIHandler();
	~UIHandler();

	void Initialize();
	void Shutdown();

	UIComponent* GetNextUIComponent();
	void UpdateUIComponentsclicked(DirectX::XMFLOAT2 mousePos);
	void UpdateUIComponentsclicked(DirectX::XMFLOAT2 mousePos, DirectX::XMFLOAT2 windowSize);
};

#endif
