#ifndef SSPEDITOR_CHECKPOINTHANDLER_CHECKPOINTHANDLER_H
#define SSPEDITOR_CHECKPOINTHANDLER_CHECKPOINTHANDLER_H
#include "LevelHeaders.h"
#include "Header.h"
#include <vector>

/*
Holds all the checkpoints used by current level.
for containers the accual levelheader entity is used. 
Uses unique entity ids

this will be cleared when a new level is created.
*/

class CheckpointHandler
{
private:
	std::vector<CheckpointContainer*> m_checkpoints;
public:
	CheckpointHandler();
	~CheckpointHandler();
	
	CheckpointContainer* GetCheckpoint(int entityID);
	std::vector<CheckpointContainer*>* GetAllCheckpoints() { return &this->m_checkpoints; };
	void removeCheckpoint(int index);

};




#endif
