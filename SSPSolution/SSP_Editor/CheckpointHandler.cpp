#include "CheckpointHandler.h"

CheckpointHandler::CheckpointHandler()
{
	DirectX::XMMATRIX * test;
	float testf[16];
	for (int i = 0; i < 16; i++)
	{
		testf[i] = i;
	}

	//memcpy(&test, testf, sizeof(float) * 16);
	test = &(DirectX::XMMATRIX)(testf);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%f \n", test->r[i].m128_f32[j]);
		}
	}


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



