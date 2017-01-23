#ifndef SSPEDITOR_CHECKPOINTCONTROLLER_CHECKPOINTCONTROLLER_H
#define SSPEDITOR_CHECKPOINTCONTROLLER_CHECKPOINTCONTROLLER_H
#include "LevelHeaders.h"
#include "../physicsDLL/PhysicsHandler.h"
/*
	Used to controll the checkpoints
	add/remove, scale and position etc..
*/


class CheckpointController
{
private:
	LevelData::CheckpointHeader * m_checkpoint;
	CheckpointController();
public:
	CheckpointController(LevelData::CheckpointHeader * checkpoint);
	~CheckpointController();

	void RemoveCheckpoint(int entityID);
	void SetComponent(LevelData::CheckpointHeader * checkpoint) { this->m_checkpoint = checkpoint; };
	void SetID(int entityID);
	void SetCheckpointNumber(int number);
	void SetOBB(OBB obb);
	void SetPosition(float x, float y, float z);
	LevelData::CheckpointHeader * GetCheckpoint() { return this->m_checkpoint; };

};


#endif