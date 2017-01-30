#include "SoundHandler.h"

SoundHandler::SoundHandler()
{
}

SoundHandler::~SoundHandler()
{
}

int SoundHandler::Initialize()
{

	this->soundEngine = irrklang::createIrrKlangDevice();

	if (!this->soundEngine)
	{
		return 0;	//Error on creation
	}

	return 1;
}

void SoundHandler::Shutdown()
{
	this->soundEngine->drop();
}
