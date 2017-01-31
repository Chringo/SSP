#include "LevelDirector.h"
#define SUCCESS 1
#define FAIL 0

FSMEnvironment::LevelDirector::LevelDirector(){}
FSMEnvironment::LevelDirector::~LevelDirector(){}
int FSMEnvironment::LevelDirector::Shutdown()
{
	this->m_states.clear();
	return SUCCESS;
}

int FSMEnvironment::LevelDirector::Initialize()
{
	// Reset values
	this->m_states.clear();

	this->m_defaultState = nullptr;
	this->m_currentState = nullptr;
	this->m_goalState = nullptr;

	// TODO: Import new states from new LevelState
#pragma region temp
	State test;
	test.stateID = 0;
	test.timeDelay = 10;
	test.hint1 = Hint::EXAMPLE;
	AddState(&test);
	test.stateID = 1;
	test.timeDelay = 15;
	test.hint1 = Hint::EXAMPLE;
	AddState(&test);
	test.stateID = 2;
	test.timeDelay = 20;
	test.hint1 = Hint::EXAMPLE;
	AddState(&test);
	for (int i = 0; i < 3; i++)
	{
		printf("%d\n", m_states[i].stateID);
	}
	SetDefaultState(&m_states[0]);
#pragma endregion

	return SUCCESS;
}
int FSMEnvironment::LevelDirector::Update(float dt)
{
	// Return if there are no states
	if (this->m_states.size() == 0)
		return SUCCESS;
	// Return if there are no current state or default state
	if ( !(this->m_currentState) )
		this->m_currentState = this->m_defaultState;
	if ( !(this->m_currentState) )
		return SUCCESS;
	
	int oldStateID = this->m_currentState->stateID;
	this->m_goalID = this->m_currentState->CheckTransitions();
	if ( this->m_goalID != oldStateID )
	{
		if ( this->ChangeState(this->m_goalID) )
		{
			this->m_currentState->Exit();
			this->m_currentState = this->m_goalState;
			this->m_currentState->Enter();
		}
		this->m_currentState->Update(dt);
	}

	return SUCCESS;
}

int FSMEnvironment::LevelDirector::React(int entityID, EVENT event)
{

	return SUCCESS;
}

void FSMEnvironment::LevelDirector::AddState(State* newState)
{
	this->m_states.push_back(*newState);
}
void FSMEnvironment::LevelDirector::SetDefaultState(State* state)
{
	this->m_defaultState = state;
}
void FSMEnvironment::LevelDirector::SetGoalID(int goal)
{
	this->m_goalID = goal;
}
bool FSMEnvironment::LevelDirector::ChangeState(int newState)
{
	bool change = false;

	// Query list of states to see if the state exists
	for (unsigned int i = 0; i < m_states.size(); i++)
	{
		if (m_states[i].stateID == newState)
		{
			change = true;
			this->m_goalState = &m_states[i];
			break;
		}
	}

	return change;
}

#pragma region temp
void FSMEnvironment::State::Initialize()
{
	stateID = -1;
	int goalStateID = 0;

	// State - Data
	playerOne = false;
	playerTwo = false;
	timeDelay = -1;
	
	for (size_t i = 0; i < 5; i++)
	{
		subjects->entityID = -1;
		subjects->used = false;
	}

	hint1 = Hint::NONE;
	hint2 = Hint::NONE;
	hint3 = Hint::NONE;
}
int FSMEnvironment::State::CheckTransitions()
{
	// TODO: check if conditions are true for goalState
	if (!playerOne && !playerTwo)
		return 0;

	return 1;	// TODO: Return stateID
}
void FSMEnvironment::State::Enter()
{
	// TODO: logic unique for enter
}
void FSMEnvironment::State::Exit()
{
	// TODO: logic unique for exit
}
void FSMEnvironment::State::Update(float dt)
{
	// TODO: Update logic for currentState
}
#pragma endregion
