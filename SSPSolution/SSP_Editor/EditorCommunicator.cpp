#include "EditorCommunicator.h"

Resources::Status Communicator::Initialize(HWND hwnd, HINSTANCE hinstance, int w, int h, bool isPreview)
{
	this->m_GraphicsHandler = new GraphicsHandler();
	this->m_hwnd = hwnd;
	this->m_hInstance = hinstance;
	this->m_Width = w;
	this->m_Height = h;
	this->m_IsPreview = isPreview;
	this->m_InternalID = 1; 

	this->m_GraphicsHandler->Initialize(
		&this->m_hwnd,
		DirectX::XMINT2(m_Width, m_Height)
	);
	this->m_GraphicsHandler->InitializeGrid();

	this->m_Camera = new Camera();
	this->m_Camera->Initialize();
	Camera* oldCam = this->m_GraphicsHandler->SetCamera(this->m_Camera);
	delete oldCam;
	oldCam = nullptr;

	if (!isPreview)
	{
		this->m_EditorInputHandler = new EditorInputHandler(
			this->m_hInstance,
			this->m_hwnd,
			this->m_Camera,
			this->m_Width,
			this->m_Height,
			this->m_GraphicsHandler
		);
	}

	this->m_ModelContainer.reserve(50);


	return Resources::ST_OK;
}

Communicator::Communicator()
{
}

Communicator::~Communicator()
{

}

Resources::Status Communicator::Release()
{
	this->m_GraphicsHandler->Shutdown();
	delete this->m_GraphicsHandler;
	this->m_ModelContainer.clear();
	if (!this->m_IsPreview)
	{
		delete this->m_EditorInputHandler;
	}
	return Resources::ST_OK;
}

Container Communicator::GetModel(unsigned int id)
{
	for (size_t i = 0; i < this->m_ModelContainer.size(); i++)
	{
		if (this->m_ModelContainer.at(i).internalId == id)
		{
			return this->m_ModelContainer.at(i);
		}
	}
}

void Communicator::AddModel(Resources::Model model, DirectX::XMVECTOR position, float rotation)
{
	Container newModel;
	newModel.component.modelID = model.GetId();
	newModel.position = position;
	newModel.rotation = rotation;
	DirectX::XMMATRIX containerMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationY(rotation);
	containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, rotationMatrix);
	containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, DirectX::XMMatrixTranslationFromVector(position));
	newModel.component.worldMatrix = containerMatrix;
	newModel.internalId = this->m_InternalID; 
	this->m_InternalID += 1; 

	this->m_ModelContainer.push_back(newModel);

}

void Communicator::UpdateModel(unsigned int id, DirectX::XMVECTOR position, float rotation)
{
	for (size_t i = 0; i < this->m_ModelContainer.size(); i++)
	{
		if (this->m_ModelContainer.at(i).internalId == id)
		{
			this->m_ModelContainer.at(i).position = position;
			this->m_ModelContainer.at(i).rotation = rotation;
			DirectX::XMMATRIX containerMatrix = DirectX::XMMatrixIdentity();
			DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationY(rotation);
			containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, rotationMatrix);
			containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, DirectX::XMMatrixTranslationFromVector(position));
			this->m_ModelContainer.at(i).component.worldMatrix = containerMatrix;
		}
	}
}

void Communicator::RemoveModel(unsigned int id)
{
	for (size_t i = 0; i < this->m_ModelContainer.size(); i++)
	{
		if (this->m_ModelContainer.at(i).internalId == id)
		{
			this->m_ModelContainer.at(i).internalId = 0;
		}
	}
}
