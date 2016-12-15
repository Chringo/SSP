#ifndef SSPAPPLICATION_AI_LEVELDIRECTOR_H
#define SSPAPPLICATION_AI_LEVELDIRECTOR_H
#include "Observer.h"
#include <vector>
namespace FSMEnvironment
{

#pragma region temp
	enum Hint
	{
		NONE = 0,
		EXAMPLE
	};
	struct State
	{
		int stateID = -1;
		int timeDelay = -1;
		Hint hint = Hint::NONE;

		int CheckTransitions();
		void Enter();
		void Exit();
		void Update(float deltaTime);
	};
#pragma endregion

	class LevelDirector
	{
	private:	// Variables
		State* m_currentState;
		State* m_defaultState;
		State* m_goalState; // A state which is the current goal for the FSM
		int m_goalID;
		std::vector<State> m_states;
	public:
		LevelDirector();
		~LevelDirector();
		int Shutdown();

		int Initialize();
		int Update(float deltaTime);

		int React(int entityID, EVENT event);


	private:	// Helper functions
		// TODO: 
		// Depending on what kind of array/vector we end up with to hold our states
		// the argument list should be updated accordingly
		void AddState(State* newState);
		void SetDefaultState(State* state);
		bool ChangeState(int state);

	};
}
#endif