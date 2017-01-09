#include "LevelSelectState.h"



LevelSelectState::LevelSelectState()
{
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

int LevelSelectState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler)
{
	int result = 0;
	result = GameState::InitializeBase(gsh, cHandler);

	if(result <= 0)
		return result;

	this->currentLevel = new LevelState();

	//Create, Initialize and push a LevelState

	result = currentLevel->Initialize(gsh, cHandler);
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
	return 0;
}

int LevelSelectState::LoadLevel(std::string path)
{

	int result = 0;
	Resources::Status st = Resources::Status::ST_OK;
	
	LevelData::Level* level;    //pointer for data
	st = Resources::FileLoader::GetInstance()->LoadLevel(path, level); //load file

	if (st != Resources::ST_OK) //if not successful
		return 0;
	 result = this->currentLevel->CreateLevel(level); //Create the level
	 if (result <= 0)
		 return result;


	return 1;
}
