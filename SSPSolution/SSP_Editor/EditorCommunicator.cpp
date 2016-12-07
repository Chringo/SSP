#include "EditorCommunicator.h"

Resources::Status Communicator::Initialize(HWND hwnd, HINSTANCE hinstance, int w, int h, bool isPreview)
{
	this->m_GraphicsHandler = new GraphicsHandler();
	this->m_hwnd = hwnd;
	this->m_hInstance = hinstance;
	this->m_Width = w;
	this->m_Height = h;
	this->m_IsPreview = isPreview;

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

	this->m_Map.reserve(200);
	for (size_t i = 0; i < this->m_Map.capacity(); i++)
	{
		this->m_Map.push_back(new std::vector<Container>);
	}
	//this->m_Map.push_back(new std::vector<Container>);


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
	for (size_t i = 0; i < this->m_Map.size(); i++)
	{
		delete this->m_Map.at(i);
	}
	if (!this->m_IsPreview)
	{
		delete this->m_EditorInputHandler;
	}
	return Resources::ST_OK;
}

Container Communicator::GetModel(unsigned int modelID, unsigned int InstanceID)
{
	for (unsigned int i = 0; i < this->m_Map.at(modelID)->size(); i++)
	{
		if (this->m_Map.at(modelID)->at(i).internalID == InstanceID)
		{
			return this->m_Map.at(modelID)->at(i);
		}
	}
}

void Communicator::AddModel(Resources::Model model, DirectX::XMVECTOR position, float rotation, unsigned int instanceID)
{
	if (this->m_Map.size() < model.GetId())
	{
		int temp = model.GetId() - this->m_Map.size();
		this->m_Map.reserve(temp);
		this->m_Map.resize(model.GetId());

		for (unsigned int i = this->m_Map.size(); i < this->m_Map.capacity(); i++)
		{
			this->m_Map.push_back(new std::vector<Container>);
		}
	}

	Container newComponent;
	newComponent.component.modelID = model.GetId();
	newComponent.position = position;
	newComponent.rotation = rotation;
	DirectX::XMMATRIX containerMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationY(rotation);
	containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, rotationMatrix);
	containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, DirectX::XMMatrixTranslationFromVector(position));
	newComponent.component.worldMatrix = containerMatrix;
	newComponent.internalID = instanceID;

	this->m_Map.at(model.GetId())->push_back(newComponent);
}

void Communicator::UpdateModel(unsigned int modelID, unsigned int InstanceID, DirectX::XMVECTOR position, float rotation)
{

	for (unsigned int i = 0; i < this->m_Map.at(modelID)->size(); i++)
	{
		if (this->m_Map.at(modelID)->at(i).internalID == InstanceID)
		{
			this->m_Map.at(modelID)->at(i).position = position;
			this->m_Map.at(modelID)->at(i).rotation = rotation;
			DirectX::XMMATRIX containerMatrix = DirectX::XMMatrixIdentity();
			DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationY(rotation);
			containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, rotationMatrix);
			containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, DirectX::XMMatrixTranslationFromVector(position));
			this->m_Map.at(modelID)->at(i).component.worldMatrix = containerMatrix;
		}
	}
}

void Communicator::RemoveModel(unsigned int modelID, unsigned int InstanceID)
{
	for (unsigned int i = 0; i < this->m_Map.at(modelID)->size(); i++)
	{
		if (this->m_Map.at(modelID)->at(i).internalID == InstanceID)
		{
			this->m_Map.at(modelID)->erase(m_Map.at(modelID)->begin() + i);
		}
	}
}
