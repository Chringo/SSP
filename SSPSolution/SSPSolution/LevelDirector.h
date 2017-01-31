#ifndef SSPAPPLICATION_AI_LEVELDIRECTOR_H
#define SSPAPPLICATION_AI_LEVELDIRECTOR_H
#include "ComponentStructs.h" // EVENT Handling in the function LevelDirector::React
#include "Observer.h"
#include <vector>
//INCLUDE sound component or handler 
namespace FSMEnvironment
{

#pragma region temp
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

	class State
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

		Hint hint1 = Hint::NONE;
		Hint hint2 = Hint::NONE;
		Hint hint3 = Hint::NONE;
		

		// Sub-states
		void Initialize();	//Reset state
		int CheckTransitions();
		void Enter();
		void Exit();
		void Update(float deltaTime);
	};
#pragma endregion

	class LevelDirector
	{
	private:	// Variables
		State* m_currentState;	// Current logical behaviour on the level
		State* m_defaultState;	// Default logical behaviour for the level
		State* m_goalState;		// A state which is the current goal for the FSM
		int m_goalID;			// ID of a state which the FSM transitions to
		std::vector<State> m_states;	// Holds the states which defines the behaviour
	public:
		LevelDirector();
		~LevelDirector();
		int Shutdown();

		int Initialize();		// Initiate values for new level, i.e. reset values and load new level
		int Update(float deltaTime);	// Update LD and check state

		int React(int entityID, EVENT event);


	private:	// Helper functions
		// TODO: 
		// Depending on what kind of array/vector we end up with to hold our states
		// the argument list should be updated accordingly
		void AddState(State* newState);		// Add new state to the FSM
		void SetDefaultState(State* state);	// Set new default state for the level director FSM, ideally on position 0
		void SetGoalID(int goal);			// Update m_goalID to new goal
		bool ChangeState(int state);		// Search for StateID and transition to it

	};
}
#endif