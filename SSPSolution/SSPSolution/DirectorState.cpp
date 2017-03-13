#include "DirectorState.h"

DirectorState::DirectorState()
{
}
DirectorState::~DirectorState()
{
}

void DirectorState::Initialize()
{
	stateID = -1;
	goalStateID = 0;

	// Field - Data
	for (size_t i = 0; i < this->nrOfFields; i++)
	{
		fieldMap[i].FD_entityID = nullptr;
		fieldMap[i].FD_first_inside = nullptr;
		fieldMap[i].FD_second_inside = nullptr;
	}

	// State - Data
	timeDelay = -1;

	hint1 = Hint::NONE;
	hint2 = Hint::NONE;
	hint3 = Hint::NONE;
}
int DirectorState::CheckTransitions()
{
	// TODO: check if conditions are true for goalState
	for (size_t i = 0; i < 3; i++)
	{
		if (this->fieldMap[i].FD_first_inside == false && this->fieldMap[i].FD_second_inside == false)
			return 0; // returning to idle state
	}

	return 1;	// TODO: Return stateID
}
void DirectorState::Enter()
{
	// TODO: logic unique for enter
}
void DirectorState::Exit()
{
	// TODO: logic unique for exit
}
void DirectorState::Update(float dt)
{
	// TODO: Update logic for currentState
}