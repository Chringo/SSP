#ifndef SSPAPPLICATION_AI_LEVELDIRECTOR_H
#define SSPAPPLICATION_AI_LEVELDIRECTOR_H
#include "Observer.h"
#include <vector>
class LevelDirector
{
private:	// Variables
	/* TEMP STATE STRUCTURE */
	static enum State
	{
		NONE = 0,
		START,
		DEFAULT,
		GOAL
	};
	State m_currentState;
	State m_defaultState;

	// Change State to State* after temp structure is removed
	std::vector<State> m_states;
public:
	LevelDirector();
	~LevelDirector();
	int Shutdown();

	int Initialize();
	int Update(float deltaTime);

	int React(int entityID, EVENT event);


private:	// Helper functions
	void AddState(State newState);
	void SetDefaultState(State state);
	void SetNewState(State ID);// temp, could be an integer holding an ID in the future
	bool ChangeState(State state);

};
#endif