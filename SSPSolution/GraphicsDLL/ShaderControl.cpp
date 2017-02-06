#include "ShaderControl.h"



ShaderControl::ShaderControl()
{
}


ShaderControl::~ShaderControl()
{
}

bool ShaderControl::Initialize(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext, const DirectX::XMINT2& resolution)
{
	this->m_Device		   = gDevice;
	this->m_DeviceContext  = gDeviceContext;
	m_shaders[DEFERRED]    = new DeferredShader();
	m_shaders[DEFERRED]->Initialize(gDevice, gDeviceContext, resolution);
	m_shaders[FINAL]	   = new FinalShader();
	m_shaders[FINAL]->Initialize(gDevice, gDeviceContext, resolution);
	m_shaders[POSTPROCESS] = new PostProcessShader();
	m_shaders[POSTPROCESS]->Initialize(gDevice, gDeviceContext, resolution);
	return true;
}

bool ShaderControl::InitializeWireframe(ID3D11Device * device)
{
	((DeferredShader*)m_shaders[DEFERRED])->InitializeGridShader(device);
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

int ShaderControl::SetBackBuffer(ID3D11RenderTargetView * backBufferRTV, ID3D11ShaderResourceView* backBufferSRV)
{
	this->m_backBufferRTV  = backBufferRTV;
	this->m_backBufferSRV  = backBufferSRV;
	((FinalShader*)m_shaders[FINAL])->SetRenderParameters(backBufferRTV,
		((DeferredShader*)m_shaders[DEFERRED])->GetShaderResourceViews()
	);
	return 0;
}

void ShaderControl::PostProcess()
{
	ID3D11RenderTargetView* rtv = this->m_backBufferRTV;
	PostProcessShader::PostEffects fx;
	bool processed = false;
	m_DeviceContext->OMSetRenderTargets(1, &m_backBufferRTV, NULL);
	for (size_t i = 0; i < PostProcessShader::NUM_TYPES; i++)
	{
		fx = PostProcessShader::PostEffects(i);
		
		//If the effect is enabled
		if ((      (PostProcessShader*)m_shaders[POSTPROCESS])->IsEnabled(fx)) {
			(      (PostProcessShader*)m_shaders[POSTPROCESS])->SetActive(PostProcessShader::PostEffects(fx));
			rtv = ((PostProcessShader*)m_shaders[POSTPROCESS])->Draw();
			processed = true;
		}
	}
	if (processed)
	{
		ID3D11ShaderResourceView* postResourceView = ((PostProcessShader*)m_shaders[POSTPROCESS])->GetActiveShaderResource();
		ID3D11Resource* postResource;
		postResourceView->GetResource(&postResource);


		ID3D11Resource* bbResource;
		m_backBufferSRV->GetResource(&bbResource);
		m_DeviceContext->CopyResource(bbResource,postResource);
		m_DeviceContext->OMSetRenderTargets(1, &m_backBufferRTV, NULL);
	}
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

void ShaderControl::Draw(Resources::Model * model, GraphicsAnimationComponent * component) // FOR FAN
{
	switch (m_activeShader)
	{
	case DEFERRED:
		((DeferredShader*)m_shaders[DEFERRED])->Draw(model, component);
		break;
	}

}

void ShaderControl::DrawInstanced(InstanceData * data)
{

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
