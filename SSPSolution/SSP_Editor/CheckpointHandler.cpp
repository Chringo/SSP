#include "CheckpointHandler.h"

CheckpointHandler::CheckpointHandler()
{
}

CheckpointHandler::~CheckpointHandler()
{
}

CheckpointContainer * CheckpointHandler::GetCheckpoint(int entityID)
{
	for each (CheckpointContainer* checkpoint in this->m_checkpoints)
	{
		if (checkpoint->internalID == entityID)
			return checkpoint;
	}

	return nullptr;
}

void CheckpointHandler::removeCheckpoint(int index)
{

}

