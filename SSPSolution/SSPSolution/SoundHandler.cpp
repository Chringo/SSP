#include "SoundHandler.h"

SoundHandler::SoundHandler()
{
}

SoundHandler::~SoundHandler()
{
}

int SoundHandler::Initialize()
{

	this->m_soundEngine = irrklang::createIrrKlangDevice();

	if (!this->m_soundEngine)
	{
		return 0;	//Error on creation
	}

	this->LoadSounds();

	return 1;
}

void SoundHandler::Shutdown()
{
	for (int i = 0; i < this->soundComponents.size(); i++) {
		delete this->soundComponents.at(i);
	}

	std::vector<irrklang::ISoundSource*>::iterator itr;
	//Clear 2D sounds
	for (itr = this->m_sounds2D.begin(); itr != this->m_sounds2D.end(); itr++)
	{
		(*itr)->drop();
	}

	//Clear 3D sounds
	for (itr = this->m_sounds3D.begin(); itr != this->m_sounds3D.end(); itr++)
	{
		(*itr)->drop();
	}

	this->m_soundEngine->drop();
}

void SoundHandler::LoadSounds()
{
	irrklang::ISoundSource* sp = nullptr;

	//Load 2D sounds
#pragma region
	//Sound 1
	sp = m_soundEngine->addSoundSourceFromFile("1.mp3");
	if (sp != nullptr)
	{
		sp->grab();
		this->m_sounds2D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	//Sound 2
	sp = m_soundEngine->addSoundSourceFromFile("2.wav");
	if (sp != nullptr)
	{
		sp->grab();
		this->m_sounds2D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

#pragma endregion 2D_Sounds

	//Load 3D sounds
#pragma region


#pragma endregion 3D_Sounds
	
	sp = nullptr;

}

int SoundHandler::PlaySound2D(Sounds2D soundEnum)
{
	//Check if the enum will fit as an index
	if (soundEnum <= this->m_sounds2D.size() - 1)
	{
		irrklang::ISoundSource* sp = this->m_sounds2D.at(soundEnum);
		this->m_soundEngine->play2D(sp);

		return 1;
	}

	return 0;
}

int SoundHandler::PlaySound3D(Sounds3D soundEnum, DirectX::XMFLOAT3 pos)
{
	//Check if the enum will fit as an index
	if (soundEnum <= this->m_sounds3D.size() - 1)
	{
		irrklang::ISoundSource* sp = this->m_sounds3D.at(soundEnum);
		irrklang::vec3d<float> pos(pos.x, pos.y, pos.z);
		this->m_soundEngine->play3D(sp, pos);

		return 1;
	}

	return 0;

}

void SoundHandler::UpdateSoundHandler()
{
	std::vector<SoundComponent*>::iterator itr;
	for (itr = this->soundComponents.begin(); itr != this->soundComponents.end(); itr++) {
		if (!(*itr)->isActive)
		{
			delete (*itr);
			itr = this->soundComponents.erase(itr);
		}
	}
}

SoundComponent * SoundHandler::GetSoundComponent()
{
	SoundComponent* scp = new SoundComponent();
	this->soundComponents.push_back(scp);
	
	return scp;
}
