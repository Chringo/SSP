#include "LevelDirector.h"
#define SUCCESS 1
#define FAIL 0

FSMEnvironment::LevelDirector::LevelDirector(){}
FSMEnvironment::LevelDirector::~LevelDirector(){}
int FSMEnvironment::LevelDirector::Shutdown()
{

	return SUCCESS;
}

int FSMEnvironment::LevelDirector::Initialize()
{
	// Reset values
	this->m_states.clear();
	// temp reset values
	this->m_currentState = NONE;
	this->m_defaultState = DEFAULT;
	// TODO: Import new states from new LevelState
#pragma region temp
	State test;
	test.stateID = 0;
	test.timeDelay = 10;
	test.hint = Hint::EXAMPLE;
	AddState(&test);
	//m_states.push_back(test);
	test.stateID = 1;
	test.timeDelay = 15;
	test.hint = Hint::EXAMPLE;
	AddState(&test);
	//m_states.push_back(test);
	test.stateID = 2;
	test.timeDelay = 20;
	test.hint = Hint::EXAMPLE;
	AddState(&test);
	//m_states.push_back(test);
	for (int i = 0; i < 3; i++)
	{
		printf("%d\n", m_states[i].stateID);
	}
	m_currentState = &m_states.front();
#pragma endregion

	return SUCCESS;
}
int FSMEnvironment::LevelDirector::Update(float dt)
{
	if (this->m_states.size() == 0)
		return SUCCESS;
	if (this->m_currentState == NONE)
		this->m_currentState = this->m_defaultState;
	//if(!(this->m_currentState))
	//	return SUCCESS;

	State oldState = this->m_currentState;
	State newState = State::DEFAULT; // please ignore
	if (State::GOAL != oldState)// temporary static goal
	{
		if (this->ChangeState(newState))
		{
			//this->m_currentState->Exit();
			//this->m_currentState = this->m_goalState;
			//this->m_currentState->Enter();
		}
		//this->m_currentState->Update(dt);
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
bool FSMEnvironment::LevelDirector::ChangeState(int newState)
{
	bool change = false;
	// Query list of states to see if the state exists
	for (int i = 0; i < m_states.size(); i++)
	{
		if (m_states[i].stateID == newState)
		{
			change = true;
			break;
		}
	}
	return change;
}

#pragma region temp
int FSMEnvironment::State::CheckTransitions()
{

	return 1;// TODO: Return ID
}
void FSMEnvironment::State::Enter()
{

}
void FSMEnvironment::State::Exit()
{

}
void FSMEnvironment::State::Update(float dt)
{

}
#pragma endregion

