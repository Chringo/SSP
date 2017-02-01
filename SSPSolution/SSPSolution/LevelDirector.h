#ifndef SSPAPPLICATION_AI_LEVELDIRECTOR_H
#define SSPAPPLICATION_AI_LEVELDIRECTOR_H
#include "Observer.h"
#include <vector>
#include "DirectorState.h"
//INCLUDE sound component or handler 
namespace FSMEnvironment
{
#define State DirectorState
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

		int React(int entityID, EVENT event);//TODO: Might be moved to state
		// Add function to attach either state or LD to an entity

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