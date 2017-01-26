#include "CheckpointHandler.h"

CheckpointHandler::CheckpointHandler()
{
}

CheckpointHandler::~CheckpointHandler()
{
	for each (CheckpointContainer* container in m_checkpoints)
	{
		delete container;
	}
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



