#include "LevelSelectState.h"
#include "GameStateHandler.h"


LevelSelectState::LevelSelectState()
{
	m_currentLevel = nullptr;
}


LevelSelectState::~LevelSelectState()
{
	/*delete m_currentLevel;
	m_currentLevel = nullptr;*/
}

int LevelSelectState::ShutDown()
{
	/*if (this->m_currentLevel != nullptr)
	{
		this->m_currentLevel->ShutDown();
		delete this->m_currentLevel;
		this->m_currentLevel = nullptr;
	}*/
	int result = 1;
	return result;
}

int LevelSelectState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler, Camera* cameraRef)
{
	int result = 0;
	result = GameState::InitializeBase(gsh, cHandler, cameraRef, false);

	if(result <= 0)
		return result;

	this->m_currentLevel = new LevelState();

	//Create, Initialize and push a LevelState
	result = m_currentLevel->Initialize(gsh, cHandler, cameraRef);

	//If the initialization was successful
	if (result <= 0)
	{
		//Delete it
		delete m_currentLevel;
		m_currentLevel = nullptr;
	}
	return result;
}

int LevelSelectState::Update(float dt, InputHandler * inputHandler)
{
	int result = 0;
	/*if (this->m_currentLevel != nullptr)
	{
		result = this->m_currentLevel->Update(dt, inputHandler);
	}
	if (result == -2)

	{
		this->m_currentLevel->ShutDown();
		delete this->m_currentLevel;
		this->m_currentLevel = nullptr;
		this->m_gsh->PopStateFromStack();
	}*/
	return result;
}

int LevelSelectState::LoadLevel(int levelID)
{

	int result = 0;
	Resources::Status st = Resources::Status::ST_OK;
	//Prepare the level state
	this->m_currentLevel->SetCurrentLevelID(levelID);
	//Get the level path from the level state for the level id/index
	std::string levelPath = this->m_currentLevel->GetLevelPath();
	
	LevelData::Level* level;    //pointer for data
	//Load LevelData from file

		st = Resources::FileLoader::GetInstance()->LoadLevel(levelPath, level); //load file
		//if not successful
		if (st != Resources::ST_OK)
			return 0;
		//Load Resources of the level
		st = Resources::ResourceHandler::GetInstance()->LoadLevel(level->resources, level->numResources);
		//if not successful
		if (st != Resources::ST_OK)
			return 0;
		
		//Load Lights of the level

		if (!LIGHTING::LightHandler::GetInstance()->LoadLevelLight(level))
			return 0;
	
	//Create level
	result = this->m_currentLevel->CreateLevel(level);
	m_cHandler->GetGraphicsHandler()->GenerateStaticSceneShadows();

#pragma region
	DirectX::XMVECTOR cubePos;
	switch (levelID)
	{
	case 0: //Tutorial
		cubePos = DirectX::XMVectorSet(0.0f, 2.0f, -19.0f, 1.0f);
		break;
	case 1:
		cubePos = DirectX::XMVectorSet(8.0f, 4.0f, -2.0f, 1.0f);
		break;
	case 2:
		cubePos = DirectX::XMVectorSet(6.0f, 3.0f, -67.0f, 1.0f);
		break;
	case 3:
		cubePos = DirectX::XMVectorSet(0.0f, 2.0f, 2.0f, 1.0f);
		break;
	case 4:
		cubePos = DirectX::XMVectorSet(10.0f, 3.0f, -22.0f, 1.0f);
		break;
	case 5:
		cubePos = DirectX::XMVectorSet(20.0f, 2.0f, 12.0f, 1.0f);
		break;
	case 6:
		cubePos = DirectX::XMVectorSet(15.0f, 5.0f, -19.0f, 1.0f);
		break;
	case 7:
		cubePos = DirectX::XMVectorSet(0.0f, 2.0f, -19.0f, 1.0f);
		break;
	default:
		cubePos = DirectX::XMVectorSet(0.0f, 2.0f, -19.0f, 1.0f);
			break;
	}

#pragma endregion Get cube map pos

	m_cHandler->GetGraphicsHandler()->GenerateSceneCubeMap(cubePos);

	this->m_gsh->PushStateToStack(this->m_currentLevel);

	return result;
}

int LevelSelectState::EnterState()
{
	return 0;
}

int LevelSelectState::LeaveState()
{
	return 0;
}
