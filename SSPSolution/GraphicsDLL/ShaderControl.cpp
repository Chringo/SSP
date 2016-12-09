#include "ShaderControl.h"



ShaderControl::ShaderControl()
{
}


ShaderControl::~ShaderControl()
{
}

void ShaderControl::Initialize(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext, const DirectX::XMINT2& resolution)
{
	this->m_Device		  = gDevice;
	this->m_DeviceContext = gDeviceContext;
	m_shaders[DEFERRED]   = new DeferredShader();
	m_shaders[DEFERRED]->Initialize(gDevice,gDeviceContext, resolution);
	m_shaders[FINAL]	  = new FinalShader();
	m_shaders[FINAL]->Initialize(gDevice, gDeviceContext, resolution);

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
}

void ShaderControl::Draw(Resources::Model * model)
{
	switch (m_activeShader)
	{
	case DEFERRED:
		m_shaders[m_activeShader]->SetActive(ShaderLib::ShaderVariations::Normal);
		((DeferredShader*)m_shaders[m_activeShader])->Draw(ShaderLib::DrawType::DRAW_STANDARD);
		break;
	}

}
