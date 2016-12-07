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


	this->m_Map.reserve(50);

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
	this->m_Map.clear();

	if (!this->m_IsPreview)
	{
		delete this->m_EditorInputHandler;
	}
	return Resources::ST_OK;
}

Resources::Status Communicator::GetModel(unsigned int modelID, unsigned int InstanceID, Container& container)
{
	std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_Map.find(modelID);
	std::vector<Container>* modelPtr;


	if (got == m_Map.end()) { // if  does not exists in memory
		return Resources::Status::ST_RES_MISSING;
	}
	else {
		modelPtr = &got->second;

		for (size_t i = 0; i < modelPtr->size(); i++)
		{
			if (modelPtr->at(i).internalID == InstanceID)
			{
				container = modelPtr->at(i);
				return Resources::Status::ST_OK;
			}

		}
		return Resources::Status::ST_RES_MISSING;
	}
}

Resources::Status Communicator::AddModel(unsigned int modelID, unsigned int instanceID, DirectX::XMVECTOR position, float rotation)
{

	std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_Map.find(modelID);
	std::vector<Container>* modelPtr;

	Container newComponent;

	newComponent.component.modelID = modelID;
	newComponent.position = position;
	newComponent.rotation = rotation;
	DirectX::XMMATRIX containerMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationY(rotation);
	containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, rotationMatrix);
	containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, DirectX::XMMatrixTranslationFromVector(position));
	newComponent.component.worldMatrix = containerMatrix;
	newComponent.internalID = instanceID;


	if (got == m_Map.end()) { // if  does not exists in memory
		this->m_Map[modelID].push_back(newComponent);
		return Resources::Status::ST_OK;
	}
	else if(got != m_Map.end()) {
		modelPtr = &got->second;
		modelPtr->push_back(newComponent);
		return Resources::Status::ST_OK;
	}
	else {
		return Resources::Status::ST_RES_MISSING;
	}


}

Resources::Status Communicator::UpdateModel(unsigned int modelID, unsigned int InstanceID, DirectX::XMVECTOR position, float rotation)
{
	std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_Map.find(modelID);
	std::vector<Container>* modelPtr;

	if (got == m_Map.end()) { // if  does not exists in memory

		return Resources::Status::ST_RES_MISSING;
	}
	else {
		modelPtr = &got->second;

		for (size_t i = 0; i < modelPtr->size(); i++)
		{
			if (modelPtr->at(i).internalID == InstanceID)
			{
				modelPtr->at(i).position = position;
				modelPtr->at(i).rotation = rotation;
				DirectX::XMMATRIX containerMatrix = DirectX::XMMatrixIdentity();
				DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationY(rotation);
				containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, rotationMatrix);
				containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, DirectX::XMMatrixTranslationFromVector(position));
				modelPtr->at(i).component.worldMatrix = containerMatrix;
				return Resources::Status::ST_OK;
			}

		}
		return Resources::Status::ST_RES_MISSING;
	}
}

Resources::Status Communicator::RemoveModel(unsigned int modelID, unsigned int InstanceID)
{
	std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_Map.find(modelID);
	std::vector<Container>* modelPtr;

	if (got == m_Map.end()) { // if  does not exists in memory

		return Resources::Status::ST_RES_MISSING;
	}
	else {
		modelPtr = &got->second;
		modelPtr->erase(modelPtr->begin() + InstanceID -1);
		return Resources::Status::ST_OK;
	}
}
