#include "LevelDirector.h"
#define SUCCESS 1
#define FAIL 0

LevelDirector::LevelDirector(){}
LevelDirector::~LevelDirector(){}
int LevelDirector::Shutdown()
{

	return SUCCESS;
}

int LevelDirector::Initialize()
{
	// Reset values
	this->m_states.clear();
	// temp reset values
	this->m_currentState = NONE;
	this->m_defaultState = DEFAULT;

	return SUCCESS;
}
int LevelDirector::Update(float dt)
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

int LevelDirector::React(int entityID, EVENT event)
{

	return SUCCESS;
}

void LevelDirector::AddState(State newState)
{
	this->m_states.push_back(newState);
}
void LevelDirector::SetDefaultState(State state)
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
