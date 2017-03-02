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

const unsigned int GlobalIDHandler::AddExistingID(unsigned int id)
{
	bool IdExists = false;
	unsigned int toReturn = id;
	if (id == 0 || id == 1) //make sure it does not contain the player ids
		IdExists = true;
	else
	{
		for (size_t i = 0; i < ids.size(); i++) //check if the id exists
		{
			if (ids.at(i) == id) // if found
			{
				IdExists = true;
				break;
			}
		}
	}

	if (currentID < id)
	{
		currentID = id;
	}

	if (IdExists) //if it exists generate a new one
	{
		toReturn = this->GetNewId();
	}
	else {
		ids.push_back(currentID);
	}

	return toReturn;
}

void GlobalIDHandler::ResetIDs()
{
	ids.clear();
	currentID = 1;
}

bool GlobalIDHandler::ReturnRemovedIndex(int index)
{
	for (int i = 0; i < ids.size(); ++i)
	{
		if (index == ids.at(i))
		{
			ids.erase((ids.begin() + i));
			return true;
		}
	}
	return false;
}