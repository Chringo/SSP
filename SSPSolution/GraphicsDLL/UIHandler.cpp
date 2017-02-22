#include "UIHandler.h"

UIHandler::UIHandler()
{
}

UIHandler::~UIHandler()
{
}

void UIHandler::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->m_maxUIComponents = 15;
	this->m_nrOfUIComponents = 0;
	for (unsigned int i = 0; i < this->m_maxUIComponents; i++)
	{
		UIComponent* newUIComp = new UIComponent;
		this->m_UIComponents.push_back(newUIComp);
	}

	this->m_maxTextComponents = 30;
	this->m_nrOfTextComponents = 0;
	for (unsigned int i = 0; i < this->m_maxTextComponents; i++)
	{
		TextComponent* newTextComp = new TextComponent;
		this->m_textComponents.push_back(newTextComp);
	}

	this->m_spriteBatch = new DirectX::SpriteBatch(deviceContext);
	this->m_spriteFont = new DirectX::SpriteFont(device, L"consolas.spritefont");
	
	this->m_nrOfTextures = 7;
	for (unsigned int i = 0; i < this->m_nrOfTextures; i++)
	{
		ID3D11ShaderResourceView* newTexture = nullptr;
		this->m_textures.push_back(newTexture);
	}

	DirectX::CreateWICTextureFromFile(device, L"cat.png", nullptr, &this->m_textures.at(0));
	DirectX::CreateWICTextureFromFile(device, L"gamelogo.png", nullptr, &this->m_textures.at(1));
	DirectX::CreateWICTextureFromFile(device, L"../../keymaps_temp.png", nullptr, &this->m_textures.at(2));
	DirectX::CreateWICTextureFromFile(device, L"menubg.png", nullptr, &this->m_textures.at(3));
	DirectX::CreateWICTextureFromFile(device, L"button.png", nullptr, &this->m_textures.at(4));
	DirectX::CreateWICTextureFromFile(device, L"crosshair.png", nullptr, &this->m_textures.at(5));
	DirectX::CreateWICTextureFromFile(device, L"crosshair_aim.png", nullptr, &this->m_textures.at(6));
}

void UIHandler::DrawUI()
{
	UIComponent* tempUIComp = nullptr;
	TextComponent* tempTextComp = nullptr;
	this->m_spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_BackToFront);
	for (unsigned int i = 0; i < this->m_nrOfUIComponents; i++)
	{
		tempUIComp = this->m_UIComponents.at(i);
		if (tempUIComp->active) 
		{
			if (tempUIComp->spriteID > 0 && tempUIComp->spriteID < this->m_textures.size())
			{
				this->m_spriteBatch->Draw(this->m_textures.at(tempUIComp->spriteID), tempUIComp->position, nullptr, DirectX::Colors::White, tempUIComp->rotation, DirectX::XMFLOAT2(0.f, 0.f), tempUIComp->scale, DirectX::SpriteEffects::SpriteEffects_None, tempUIComp->layerDepth);
			}
			else
			{
				this->m_spriteBatch->Draw(this->m_textures.at(0), tempUIComp->position, nullptr, DirectX::Colors::White, tempUIComp->rotation, DirectX::XMFLOAT2(0.f, 0.f), tempUIComp->scale, DirectX::SpriteEffects::SpriteEffects_None, tempUIComp->layerDepth);
			}
		}
	}
	for (unsigned int i = 0; i < this->m_nrOfTextComponents; i++)
	{
		tempTextComp = this->m_textComponents.at(i);
		if (tempTextComp->active)
		{
			this->m_spriteFont->DrawString(this->m_spriteBatch, tempTextComp->text.c_str(), tempTextComp->position, DirectX::Colors::White, tempTextComp->rotation, DirectX::XMFLOAT2(0.f, 0.f), tempTextComp->scale, DirectX::SpriteEffects::SpriteEffects_None, tempTextComp->layerDepth);
		}
	}
	this->m_spriteBatch->End();
}

void UIHandler::Shutdown()
{
	for (unsigned int i = 0; i < this->m_maxUIComponents; i++)
	{
		delete this->m_UIComponents.at(i);
	}
	for (unsigned int i = 0; i < this->m_maxTextComponents; i++)
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
	for (ID3D11ShaderResourceView* text : this->m_textures)
	{
		if (text != nullptr)
		{
			text->Release();
			text = nullptr;
		}
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
