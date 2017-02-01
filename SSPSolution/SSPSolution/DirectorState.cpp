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
int DirectorState::CheckTransitions()
{
	// TODO: check if conditions are true for goalState
	if (!playerOne && !playerTwo)
		return 0;

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