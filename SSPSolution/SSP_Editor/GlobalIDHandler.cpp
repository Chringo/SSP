#include "GlobalIDHandler.h"



GlobalIDHandler::GlobalIDHandler()
{
}


GlobalIDHandler::~GlobalIDHandler()
{
}

GlobalIDHandler * GlobalIDHandler::GetInstance()
{
	static GlobalIDHandler instance;
	return &instance;
}

const unsigned int GlobalIDHandler::GetNewId()
{
	currentID += 1;
	ids.push_back(currentID);
	return currentID;
}

void GlobalIDHandler::ResetIDs()
{
	ids.clear();
	currentID = 1;
}
