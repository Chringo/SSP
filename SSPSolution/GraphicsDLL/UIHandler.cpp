#include "UIHandler.h"

UIHandler::UIHandler()
{
}

UIHandler::~UIHandler()
{
}

void UIHandler::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->m_maxUIComponents = 28;
	this->m_nrOfUIComponents = 0;
	for (unsigned int i = 0; i < this->m_maxUIComponents; i++)
	{
		UIComponent* newUIComp = new UIComponent;
		this->m_UIComponents.push_back(newUIComp);
	}

	this->m_maxTextComponents = 35;
	this->m_nrOfTextComponents = 0;
	for (unsigned int i = 0; i < this->m_maxTextComponents; i++)
	{
		TextComponent* newTextComp = new TextComponent;
		this->m_textComponents.push_back(newTextComp);
	}

	this->m_spriteBatch = new DirectX::SpriteBatch(deviceContext);
	this->m_spriteFont = new DirectX::SpriteFont(device, L"../Assets/UIelements/consolas.spritefont");
	
	this->m_nrOfTextures = 16;
	for (unsigned int i = 0; i < this->m_nrOfTextures; i++)
	{
		ID3D11ShaderResourceView* newTexture = nullptr;
		this->m_textures.push_back(newTexture);
	}

	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/Sprites/gamelogo.png", nullptr, &this->m_textures.at(0));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/Sprites/background.png", nullptr, &this->m_textures.at(1));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/Sprites/charswide.png", nullptr, &this->m_textures.at(2));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/Sprites/cog.png", nullptr, &this->m_textures.at(3));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/Sprites/button.png", nullptr, &this->m_textures.at(4));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/Sprites/crosshair_aim.png", nullptr, &this->m_textures.at(5));	
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/Sprites/Controls.png", nullptr, &this->m_textures.at(6));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/Sprites/combinedframes.png", nullptr, &this->m_textures.at(7));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/Sprites/level0.png", nullptr, &this->m_textures.at(8));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/Sprites/level1.png", nullptr, &this->m_textures.at(9));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/Sprites/level2.png", nullptr, &this->m_textures.at(10));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/Sprites/level3.png", nullptr, &this->m_textures.at(11));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/Sprites/level4.png", nullptr, &this->m_textures.at(12));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/Sprites/level5.png", nullptr, &this->m_textures.at(13));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/Sprites/levelframe.png", nullptr, &this->m_textures.at(14));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/Sprites/Header.png", nullptr, &this->m_textures.at(15));
	
	/*DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/cat.png", nullptr, &this->m_textures.at(0));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/gamelogo.png", nullptr, &this->m_textures.at(1));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/keymaps_temp.png", nullptr, &this->m_textures.at(2));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/menubg.png", nullptr, &this->m_textures.at(3));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/button.png", nullptr, &this->m_textures.at(4));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/crosshair.png", nullptr, &this->m_textures.at(5));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/crosshair_aim.png", nullptr, &this->m_textures.at(6));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/level0.png", nullptr, &this->m_textures.at(7));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/level1.png", nullptr, &this->m_textures.at(8));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/level2.png", nullptr, &this->m_textures.at(9));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/level3.png", nullptr, &this->m_textures.at(10));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/level4.png", nullptr, &this->m_textures.at(11));
	DirectX::CreateWICTextureFromFile(device, L"../Assets/UIelements/level5.png", nullptr, &this->m_textures.at(12));*/
	

	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
	BlendState.RenderTarget[0].BlendEnable = TRUE;
	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	device->CreateBlendState(&BlendState, &this->m_blendState);
}

void UIHandler::DrawUI()
{
	UIComponent* tempUIComp = nullptr;
	TextComponent* tempTextComp = nullptr;
	this->m_spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_BackToFront, this->m_blendState);
	for (unsigned int i = 0; i < this->m_nrOfUIComponents; i++)
	{
		tempUIComp = this->m_UIComponents.at(i);
		if (tempUIComp->active) 
		{
			if (tempUIComp->spriteID > 0 && tempUIComp->spriteID < this->m_textures.size())
			{
				this->m_spriteBatch->Draw(this->m_textures.at(tempUIComp->spriteID), tempUIComp->position, nullptr, DirectX::Colors::White, tempUIComp->rotation, tempUIComp->origin, tempUIComp->scale, DirectX::SpriteEffects::SpriteEffects_None, tempUIComp->layerDepth);
			}
			else
			{
				this->m_spriteBatch->Draw(this->m_textures.at(0), tempUIComp->position, nullptr, DirectX::Colors::White, tempUIComp->rotation, tempUIComp->origin, tempUIComp->scale, DirectX::SpriteEffects::SpriteEffects_None, tempUIComp->layerDepth);
			}
		}
	}
	for (unsigned int i = 0; i < this->m_nrOfTextComponents; i++)
	{
		tempTextComp = this->m_textComponents.at(i);
		if (tempTextComp->active)
		{
			if(!tempTextComp->useBlackText)
				this->m_spriteFont->DrawString(this->m_spriteBatch, tempTextComp->text.c_str(), tempTextComp->position, DirectX::Colors::White, tempTextComp->rotation, tempTextComp->origin, tempTextComp->scale, DirectX::SpriteEffects::SpriteEffects_None, tempTextComp->layerDepth);
			else
				this->m_spriteFont->DrawString(this->m_spriteBatch, tempTextComp->text.c_str(), tempTextComp->position, DirectX::Colors::Black, tempTextComp->rotation, tempTextComp->origin, tempTextComp->scale, DirectX::SpriteEffects::SpriteEffects_None, tempTextComp->layerDepth);
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
		this->m_nrOfUIComponents++;
		return this->m_UIComponents.at(this->m_nrOfUIComponents - 1);
	}
	return nullptr;
}

int UIHandler::RemoveUIComponent(UIComponent * ptr)
{
	if (ptr)
	{
		size_t nrOfUIComps = this->m_UIComponents.size();
		for (size_t i = 0; i < nrOfUIComps; i++)
		{
			if (ptr == this->m_UIComponents.at(i))
			{
				delete this->m_UIComponents.at(i);
				this->m_UIComponents.at(i) = nullptr;
				if (i < this->m_nrOfUIComponents - 1) 
				{
					for (size_t k = i; k < nrOfUIComps - 1; k++)
					{
						this->m_UIComponents.at(k) = this->m_UIComponents.at(k + 1);
					}
				}
				UIComponent* newUIComp = new UIComponent;
				this->m_UIComponents.at(this->m_nrOfUIComponents - 1) = newUIComp;
				this->m_nrOfUIComponents--;
				return 1;
			}
		}
	}

	return 0;
}

int UIHandler::RemoveLastUIComponent()
{
	this->m_nrOfUIComponents--;
	this->m_UIComponents.at(this->m_nrOfUIComponents)->ResetValuesToDefault();
	return this->m_nrOfUIComponents;
}

TextComponent* UIHandler::GetNextTextComponent()
{
	if (this->m_nrOfTextComponents < this->m_maxTextComponents)
	{
		return this->m_textComponents.at(this->m_nrOfTextComponents++);
	}
	return nullptr;
}

int UIHandler::RemoveLastTextComponent()
{
	this->m_nrOfTextComponents--;
	this->m_textComponents.at(this->m_nrOfTextComponents)->ResetValuesToDefault();
	return this->m_nrOfTextComponents;
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
