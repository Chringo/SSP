#include "LevelDirector.h"
#define SUCCESS 1
#define FAIL 0

FSMLevel::LevelDirector::LevelDirector(){}
FSMLevel::LevelDirector::~LevelDirector(){}
int FSMLevel::LevelDirector::Shutdown()
{

	return SUCCESS;
}

int FSMLevel::LevelDirector::Initialize()
{

	return SUCCESS;
}
int FSMLevel::LevelDirector::Update(float dt)
{

	return SUCCESS;
}

int FSMLevel::LevelDirector::React(int entityID, EVENT event)
{

	return SUCCESS;
}

void FSMLevel::LevelDirector::ChangeState(States newState)
{

}
