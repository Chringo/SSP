#include "GraphicsHandler.h"

GraphicsHandler::GraphicsHandler()
{
}


GraphicsHandler::~GraphicsHandler()
{
}

int GraphicsHandler::Initialize(HWND * windowHandle, const DirectX::XMFLOAT2& resolution)
{
	this->d3dHandler = new Direct3DHandler;
	if (this->d3dHandler->Initialize(windowHandle, resolution))
	{
		return 1;
	}

	this->deferredSH = new DeferredShaderHandler;
	if (this->deferredSH->Initialize(this->d3dHandler->GetDevice(), windowHandle, resolution))
	{
		return 1;
	}

	this->lightSH = new LightShaderHandler;
	if (this->lightSH->Initialize(this->d3dHandler->GetDevice(), windowHandle, resolution))
	{
		return 1;
	}

	return 0;
}

int GraphicsHandler::Render(const DirectX::XMMATRIX& viewMatrix, const DirectX::XMFLOAT3& cameraPos)
{
	return 0;
}

void GraphicsHandler::Shutdown()
{
	if (this->d3dHandler)
	{
		this->d3dHandler->Shutdown();
		this->d3dHandler = nullptr;
	}
	if (this->deferredSH)
	{
		this->deferredSH->Shutdown();
		this->deferredSH = nullptr;
	}
	if (this->lightSH)
	{
		this->lightSH->Shutdown();
		this->lightSH = nullptr;
	}
}
