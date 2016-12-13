#ifndef SSPAPPLICATION_AI_LEVELDIRECTOR_H
#define SSPAPPLICATION_AI_LEVELDIRECTOR_H
#include "Observer.h"
#include <vector>
namespace FSMLevel
{
	enum States
	{
		
	};
	class LevelDirector
	{
	private:	// Variables
		//std::vector<"hints">

	public:
		LevelDirector();
		~LevelDirector();
		int Shutdown();

		int Initialize();
		int Update(float deltaTime);

		int React(int entityID, EVENT event);


	private:	// Helper functions
		void ChangeState(States newState);
	};
}
#endif