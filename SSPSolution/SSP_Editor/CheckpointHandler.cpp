#include "CheckpointHandler.h"

CheckpointHandler::CheckpointHandler()
{
}

CheckpointHandler::~CheckpointHandler()
{
}

LevelData::CheckpointHeader * CheckpointHandler::GetCheckpoint(int entityID)
{
	for each (LevelData::CheckpointHeader* checkpoint in this->m_checkpoints)
	{
		if (checkpoint->entityID == entityID)
			return checkpoint;
	}

	return nullptr;
}

