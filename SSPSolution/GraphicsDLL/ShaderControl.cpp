#include "ShaderControl.h"



ShaderControl::ShaderControl()
{
}


ShaderControl::~ShaderControl()
{
}

bool ShaderControl::Initialize(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext, const DirectX::XMINT2& resolution)
{
	this->m_Device		  = gDevice;
	this->m_DeviceContext = gDeviceContext;
	m_shaders[DEFERRED]   = new DeferredShader();
	m_shaders[DEFERRED]->Initialize(gDevice, gDeviceContext, resolution);
	m_shaders[FINAL]	  = new FinalShader();
	m_shaders[FINAL]->Initialize(gDevice, gDeviceContext, resolution);
	
	return true;
}

void ShaderControl::Release()
{
	for (size_t i = 0; i < NUM_SHADERS; i++)
	{
		m_shaders[i]->Release();
		delete m_shaders[i];
		m_shaders[i] = nullptr;
	}
}

void ShaderControl::SetActive(Shaders type)
{
	m_activeShader = type;
	switch (m_activeShader)
	{
		case DEFERRED:
		{
			m_shaders[DEFERRED]->SetActive();
			break;
		}
	}
}

void ShaderControl::SetVariation(ShaderLib::ShaderVariations ShaderVariations)
{
	switch (m_activeShader)
	{
		case DEFERRED:
		{
			m_shaders[DEFERRED]->SetVariation(ShaderVariations);
			break;
		}
	}
}

int ShaderControl::SetBackBufferRTV(ID3D11RenderTargetView * backBufferRTV)
{
	
	((FinalShader*)m_shaders[FINAL])->SetRenderParameters(backBufferRTV,
		((DeferredShader*)m_shaders[DEFERRED])->GetShaderResourceViews()
	);
	return 0;
}

void ShaderControl::Draw(Resources::Model * model)
{
	switch (m_activeShader)
	{
	case DEFERRED:
		((DeferredShader*)m_shaders[DEFERRED])->Draw(model);
		break;
	}
}

void ShaderControl::Draw(Resources::Model * model, GraphicsComponent * component)
{
	switch (m_activeShader)
	{
	case DEFERRED:
		((DeferredShader*)m_shaders[DEFERRED])->Draw(model, component);
		break;
	}
}

void ShaderControl::Draw(Resources::Model * model, penis * component)
{
	switch (m_activeShader)
	{
	case DEFERRED:
		((DeferredShader*)m_shaders[DEFERRED])->Draw(model, component);
		break;
	}

}

void ShaderControl::DrawFinal()
{
	this->m_activeShader = Shaders::FINAL;
	((FinalShader*)this->m_shaders[FINAL])->SetActive();
	((FinalShader*)this->m_shaders[FINAL])->Draw();
}

int ShaderControl::ClearFrame()
{

	((DeferredShader*)this->m_shaders[DEFERRED])->Clear();
	return 0;
}
