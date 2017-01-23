#include "CheckpointController.h"

CheckpointController::CheckpointController()
{
}


CheckpointController::CheckpointController(LevelData::CheckpointHeader * checkpoint)
{
	this->m_checkpoint = checkpoint;
}

CheckpointController::~CheckpointController()
{

}

void CheckpointController::RemoveCheckpoint(int index)
{

}

void CheckpointController::SetID(int entityID)
{
	this->m_checkpoint->entityID = entityID;
}

void CheckpointController::SetCheckpointNumber(int number)
{
	this->m_checkpoint->checkpointNumber = number;
}

void CheckpointController::SetOBB(OBB obb)
{
	this->m_checkpoint->obb = obb;
}

void CheckpointController::SetPosition(float x, float y, float z)
{
	this->m_checkpoint->position[0] = x;
	this->m_checkpoint->position[1] = y;
	this->m_checkpoint->position[2] = z;
}

