#include "LevelSelectState.h"



LevelSelectState::LevelSelectState()
{
	currentLevel = nullptr;
}


LevelSelectState::~LevelSelectState()
{
	delete currentLevel;
	currentLevel = nullptr;
}

int LevelSelectState::ShutDown()
{
	this->currentLevel->ShutDown();
	int result = 1;
	return result;
}

int LevelSelectState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler, Camera* cameraRef)
{
	int result = 0;
	result = GameState::InitializeBase(gsh, cHandler, cameraRef);

	if(result <= 0)
		return result;

	this->currentLevel = new LevelState();

	//Create, Initialize and push a LevelState
	result = currentLevel->Initialize(gsh, cHandler, cameraRef);

	//If the initialization was successful
	if (result <= 0)
	{
		//Delete it
		delete currentLevel;
		currentLevel = nullptr;
	}
	return result;
}

int LevelSelectState::Update(float dt, InputHandler * inputHandler)
{
	this->currentLevel->Update(dt, inputHandler);
	return 1;
}

int LevelSelectState::LoadLevel(std::string path)
{

	int result = 0;
	Resources::Status st = Resources::Status::ST_OK;
	
	LevelData::Level* level;    //pointer for data
	//Load LevelData from file
	st = Resources::FileLoader::GetInstance()->LoadLevel(path, level); //load file
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
	result = this->currentLevel->CreateLevel(level); 


	return result;
}
