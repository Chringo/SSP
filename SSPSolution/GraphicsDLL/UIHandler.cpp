#include "UIHandler.h"

UIHandler::UIHandler()
{
}

UIHandler::~UIHandler()
{
}

void UIHandler::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->m_maxUIComponents = 10;
	this->m_nrOfUIComponents = 0;
	for (int i = 0; i < this->m_maxUIComponents; i++)
	{
		UIComponent* newUIComp = new UIComponent;
		this->m_UIComponents.push_back(newUIComp);
	}

	this->m_maxTextComponents = 10;
	this->m_nrOfTextComponents = 0;
	for (int i = 0; i < this->m_maxTextComponents; i++)
	{
		TextComponent* newTextComp = new TextComponent;
		this->m_textComponents.push_back(newTextComp);
	}

	this->m_spriteBatch = new DirectX::SpriteBatch(deviceContext);
	this->m_spriteFont = new DirectX::SpriteFont(device, L"testfont.spritefont");
	DirectX::CreateWICTextureFromFile(device, L"cat.png", nullptr, &this->m_texture);

	this->m_UIComponents.at(0)->position = DirectX::XMFLOAT2(10.f, 10.f);
	this->m_UIComponents.at(0)->active = true;
	this->m_nrOfUIComponents++;

	this->m_UIComponents.at(1)->position = DirectX::XMFLOAT2(200.f, 100.f);
	this->m_UIComponents.at(1)->scale = 0.5f;
	this->m_UIComponents.at(1)->rotation = 2.0f;
	this->m_UIComponents.at(1)->active = true;
	this->m_nrOfUIComponents++;

	this->m_textComponents.at(0)->active = true;
	this->m_textComponents.at(0)->text = L"Hello";
	this->m_nrOfTextComponents++;
	
	this->m_textComponents.at(1)->active = true;
	this->m_textComponents.at(1)->text = L"World";
	this->m_textComponents.at(1)->position = DirectX::XMFLOAT2(20.f, 100.f);
	this->m_nrOfTextComponents++;
}

void UIHandler::DrawUI()
{
	UIComponent* tempUIComp = nullptr;
	TextComponent* tempTextComp = nullptr;
	this->m_spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_BackToFront);
	for (int i = 0; i < this->m_nrOfUIComponents; i++)
	{
		tempUIComp = this->m_UIComponents.at(i);
		this->m_spriteBatch->Draw(this->m_texture, tempUIComp->position, nullptr, DirectX::Colors::White, tempUIComp->rotation, DirectX::XMFLOAT2(0.f, 0.f), tempUIComp->scale, DirectX::SpriteEffects::SpriteEffects_None, tempUIComp->layerDepth);
	}
	for (int i = 0; i < this->m_nrOfTextComponents; i++)
	{
		tempTextComp = this->m_textComponents.at(i);
		this->m_spriteFont->DrawString(this->m_spriteBatch, tempTextComp->text.c_str(), tempTextComp->position, DirectX::Colors::White, tempTextComp->rotation, DirectX::XMFLOAT2(0.f, 0.f), tempTextComp->scale, DirectX::SpriteEffects::SpriteEffects_None, tempTextComp->layerDepth);
	}
	this->m_spriteBatch->End();
}

void UIHandler::Shutdown()
{
	for (int i = 0; i < this->m_maxUIComponents; i++)
	{
		delete this->m_UIComponents.at(i);
	}
	for (int i = 0; i < this->m_maxTextComponents; i++)
	{
		delete this->m_textComponents.at(i);
	}
	//this->m_spriteBatch.release();
	if (this->m_spriteBatch)
	{
		delete this->m_spriteBatch;
		this->m_spriteBatch = nullptr;
	}
	if (this->m_spriteFont)
	{
		delete this->m_spriteFont;
		this->m_spriteFont = nullptr;
	}
	if (this->m_texture)
	{
		this->m_texture->Release();
		this->m_texture = nullptr;
	}
}

UIComponent* UIHandler::GetNextUIComponent()
{
	if (this->m_nrOfUIComponents < this->m_maxUIComponents)
	{
		return this->m_UIComponents.at(this->m_nrOfUIComponents++);
	}
	return nullptr;
}

TextComponent* UIHandler::GetNextTextComponent()
{
	if (this->m_nrOfTextComponents < this->m_maxTextComponents)
	{
		return this->m_textComponents.at(this->m_nrOfTextComponents++);
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
