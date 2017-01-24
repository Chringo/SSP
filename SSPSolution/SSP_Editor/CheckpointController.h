#ifndef SSPEDITOR_CHECKPOINTCONTROLLER_CHECKPOINTCONTROLLER_H
#define SSPEDITOR_CHECKPOINTCONTROLLER_CHECKPOINTCONTROLLER_H
#include "Header.h"
#include "../physicsDLL/PhysicsHandler.h"
/*
	Used to controll the checkpoints
	add/remove, scale and position etc..
*/


class CheckpointController
{
private:
	CheckpointContainer * m_checkpoint;
	CheckpointController();
public:
	CheckpointController(CheckpointContainer * checkpoint);
	~CheckpointController();

	void SetComponent(CheckpointContainer * checkpoint) { this->m_checkpoint = checkpoint; };
	void SetID(int entityID);
	void SetCheckpointNumber(int number);
	void SetExt(float ext[3]);
	void SetOrt(float ort[16]);
	void SetPosition(float x, float y, float z);
	CheckpointContainer * GetCheckpoint() { return this->m_checkpoint; };

};


#endif