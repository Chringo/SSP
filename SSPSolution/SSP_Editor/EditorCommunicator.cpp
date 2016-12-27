#include "EditorCommunicator.h"


Resources::Status Communicator::Initialize(
	HWND hwnd,
	HINSTANCE hinstance,
	int w, int h,
	bool isPreview,
	std::vector<Resources::Model*>* modelPtr
)
{
	this->m_currentLevel = LevelHandler::GetInstance()->GetCurrentLevel();
	this->m_GraphicsHandler = new GraphicsHandler();
	this->m_hwnd = hwnd;
	this->m_hInstance = hinstance;
	this->m_Width = w;
	this->m_Height = h;
	this->m_IsPreview = isPreview;

	this->m_GraphicsHandler->Initialize(
		&this->m_hwnd,
		DirectX::XMINT2(1920.0f, 1080.0f), true
	);
	this->m_GraphicsHandler->InitializeGrid();

	this->m_Camera = new Camera();
	this->m_Camera->Initialize(this->m_Width / this->m_Height);
	Camera* oldCam = this->m_GraphicsHandler->SetCamera(this->m_Camera);
	delete oldCam;
	oldCam = nullptr;
	this->m_Camera->UpdateProjection();
	this->m_Camera->Update();

	if (!isPreview)
	{
		this->m_EditorInputHandler = new EditorInputHandler(
			this->m_hInstance,
			this->m_hwnd,
			this->m_Camera,
			this->m_Width,
			this->m_Height,
			this->m_GraphicsHandler,
			this->m_currentLevel,
			modelPtr
		);
	}


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
	delete this->m_Camera;
	if (!this->m_IsPreview)
	{
		delete this->m_EditorInputHandler;
	}
	return Resources::ST_OK;
}

Resources::Status Communicator::FindModel(int modelID, std::vector<Container>* modelPtr)
{
	//std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_ModelMap.find(modelID);
	//
	//if (got == m_ModelMap.end()) { // if  does not exists in memory
	//	return Resources::Status::ST_RES_MISSING;
	//}
	//else {
	//	modelPtr = &got->second;
		return Resources::Status::ST_OK;
	//}
}

Resources::Status Communicator::GetComponent(unsigned int modelID, unsigned int instanceID, Container& container)
{
	return m_currentLevel->GetModelEntity(modelID, instanceID, container);
}

Resources::Status Communicator::AddModel(unsigned int modelID, unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation)
{
	return m_currentLevel->AddModelEntity(modelID, instanceID, position, rotation);
}

Resources::Status Communicator::UpdateModel(unsigned int modelID, unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation)
{
	return m_currentLevel->UpdateModel(modelID, instanceID, position, rotation);
}

Resources::Status Communicator::RemoveModel(unsigned int modelID, unsigned int instanceID)
{
	return m_currentLevel->RemoveModel(modelID, instanceID);
}

void Communicator::ViewPortChanged(float height, float width)
{
	if (height != 0) {
		this->m_Camera->UpdateProjection(width / height);
		this->m_Camera->Update();
		this->m_EditorInputHandler->ViewPortChanged(height, width);
	}
}
