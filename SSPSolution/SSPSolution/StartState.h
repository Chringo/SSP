#ifndef SSPAPPLICATION_GAMESTATES_STARTSTATE_H
#define SSPAPPLICATION_GAMESTATES_STARTSTATE_H
#include "GameState.h"
class StartState :
	public GameState
{
public:
	StartState();
	virtual ~StartState();

	int Initialize();
};

#endif