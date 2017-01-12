#include "UIHandler.h"

UIHandler::UIHandler()
{
}

UIHandler::~UIHandler()
{
}

void UIHandler::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->m_maxComponents = 10;
	this->m_nrOfComponents = 0;
	for (int i = 0; i < this->m_maxComponents; i++)
	{
		UIComponent* newUIComp = new UIComponent;
		this->m_UIComponents.push_back(newUIComp);
	}

	this->m_spriteBatch = new DirectX::SpriteBatch(deviceContext);
	DirectX::CreateWICTextureFromFile(device, L"cat.png", nullptr, &this->m_texture);

	this->m_UIComponents.at(0)->position = DirectX::XMFLOAT2(10.f, 10.f);
	this->m_UIComponents.at(0)->active = true;
	this->m_nrOfComponents++;

	this->m_UIComponents.at(1)->position = DirectX::XMFLOAT2(200.f, 100.f);
	this->m_UIComponents.at(1)->scale = 0.5f;
	this->m_UIComponents.at(1)->rotation = 2.0f;
	this->m_UIComponents.at(1)->active = true;
	this->m_nrOfComponents++;
}

void UIHandler::DrawUI()
{
	UIComponent* tempComp = nullptr;
	this->m_spriteBatch->Begin();
	for (int i = 0; i < this->m_nrOfComponents; i++)
	{
		tempComp = this->m_UIComponents.at(i);
		this->m_spriteBatch->Draw(this->m_texture, tempComp->position, nullptr, DirectX::Colors::White, tempComp->rotation, DirectX::XMFLOAT2(0.f, 0.f), tempComp->scale);
	}
	this->m_spriteBatch->End();
}

void UIHandler::Shutdown()
{
	for (int i = 0; i < this->m_maxComponents; i++)
	{
		delete this->m_UIComponents.at(i);
	}
	//this->m_spriteBatch.release();
	if (this->m_spriteBatch)
	{
		delete this->m_spriteBatch;
		this->m_spriteBatch = nullptr;
	}
	if (this->m_texture)
	{
		this->m_texture->Release();
		this->m_texture = nullptr;
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
