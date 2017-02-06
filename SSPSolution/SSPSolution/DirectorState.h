#ifndef SSPAPPLICATION_AI_DIRECTORSTATE_H
#define SSPAPPLICATION_AI_DIRECTORSTATE_H
#include "ComponentStructs.h" // EVENT Handling in the function LevelDirector::React
// Different types and levels of hints per state 
enum Hint : unsigned int
{
	NONE = 0,
	ONE,
	TWO,
	THREE,
	EXAMPLE,
	TUTORIAL
};

// Handling the EVENT from LevelDirector::React
/* This is to know which entities have been discovered/used in the level so we can trigger
the next state if all these entities are part of the conditions */
struct Condition
{
	int entityID = -1;	// Which subject
	bool used = true;	// Have the players used the entity
};

struct FieldData
{
	int* FD_entityID;
	bool* FD_first_inside;
	bool* FD_second_inside;
};

class DirectorState
{
public:
	int stateID = -1;
	int goalStateID = 0; // all states go back to idle before entering other states
	// TODO: Data that allows the functions to be generic

	// State - Data
	/* both players outside the "checkpoint" (bounding box) would trigger the idle state */
	bool playerOne = false;
	bool playerTwo = false;
	int timeDelay = -1;

	Condition subjects[5];
	FieldData fieldMap[3];// Locate players in which field they are

	Hint hint1 = Hint::NONE;
	Hint hint2 = Hint::NONE;
	Hint hint3 = Hint::NONE;
public:
	DirectorState();
	~DirectorState();
	// Sub-states
	void Initialize();	//Reset state
	int CheckTransitions();
	void Enter();
	void Exit();
	void Update(float deltaTime);
};
#endif