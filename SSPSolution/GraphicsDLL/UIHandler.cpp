#include "UIHandler.h"

UIHandler::UIHandler()
{
}

UIHandler::~UIHandler()
{
}

void UIHandler::Initialize(ID3D11DeviceContext* deviceContext)
{
	this->m_maxComponents = 10;
	this->m_nrOfComponents = 0;
	for (int i = 0; i < this->m_maxComponents; i++)
	{
		UIComponent* newUIComp = new UIComponent;
		this->m_UIComponents.push_back(newUIComp);
	}

	this->m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(deviceContext);
}

void UIHandler::Shutdown()
{
	for (int i = 0; i < this->m_maxComponents; i++)
	{
		delete this->m_UIComponents.at(i);
	}
}

UIComponent* UIHandler::GetNextUIComponent()
{
	if (this->m_nrOfComponents < this->m_maxComponents)
	{
		return this->m_UIComponents.at(this->m_nrOfComponents++);
	}
	return nullptr;
}

void UIHandler::UpdateUIComponentsclicked(DirectX::XMFLOAT2 mousePos)
{
	std::vector<UIComponent*>::iterator uiCompIter;
	for (uiCompIter = this->m_UIComponents.begin(); uiCompIter != this->m_UIComponents.end(); uiCompIter++)
	{
		(*uiCompIter)->UpdateClicked(mousePos);
	}
}

void UIHandler::UpdateUIComponentsclicked(DirectX::XMFLOAT2 mousePos, DirectX::XMFLOAT2 windowSize)
{
	std::vector<UIComponent*>::iterator uiCompIter;
	for (uiCompIter = this->m_UIComponents.begin(); uiCompIter != this->m_UIComponents.end(); uiCompIter++)
	{
		(*uiCompIter)->UpdateClicked(mousePos, windowSize);
	}
}
