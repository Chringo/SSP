#include "CheckpointController.h"

CheckpointController::CheckpointController()
{
}


CheckpointController::CheckpointController(CheckpointContainer * checkpoint)
{
	this->m_checkpoint = checkpoint;
}

CheckpointController::~CheckpointController()
{

}

void CheckpointController::SetID(int entityID)
{
	this->m_checkpoint->checkpointHeader.entityID = entityID;
}

void CheckpointController::SetCheckpointNumber(int number)
{
	this->m_checkpoint->checkpointHeader.checkpointNumber = number;
}

void CheckpointController::SetExt(float ext[3])
{
	this->m_checkpoint->checkpointHeader.ext[0] = ext[0];
	this->m_checkpoint->checkpointHeader.ext[1] = ext[1];
	this->m_checkpoint->checkpointHeader.ext[2] = ext[2];
}

void CheckpointController::SetOrt(float ort[16])
{
	for (int i = 0; i < 16; i++)
	{
		this->m_checkpoint->checkpointHeader.ort[i] = ort[i];
	}
}

void CheckpointController::SetPosition(float x, float y, float z)
{
	this->m_checkpoint->checkpointHeader.position[0] = x;
	this->m_checkpoint->checkpointHeader.position[1] = y;
	this->m_checkpoint->checkpointHeader.position[2] = z;
}

