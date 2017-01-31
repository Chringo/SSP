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
	//2D components
	for (int i = 0; i < this->sound2DComponents.size(); i++) {
		delete this->sound2DComponents.at(i);
	}

	//3D components
	for (int i = 0; i < this->sound3DComponents.size(); i++) {
		delete this->sound3DComponents.at(i);
	}

	std::list<irrklang::ISound*>::iterator itrS;
	//Active sounds
	for (itrS = this->m_activeSounds.begin(); itrS != this->m_activeSounds.end(); itrS++)
	{
		(*itrS)->drop();
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
	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/menu1.mp3");
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
	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/menu2.mp3");
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

int SoundHandler::PlaySound2D(Sounds2D soundEnum, bool loop)
{
	//Check if the enum will fit as an index
	if (soundEnum < this->m_sounds2D.size() && soundEnum != Sounds2D::NO_SOUND2D)
	{
		irrklang::ISoundSource* sp = this->m_sounds2D.at(soundEnum);
		irrklang::ISound* newActiveSound = this->m_soundEngine->play2D(sp, loop, false, true);
		
		newActiveSound->grab();
		this->m_activeSounds.push_back(newActiveSound);

		return 1;
	}

	return 0;
}

int SoundHandler::PlaySound3D(Sounds3D soundEnum, DirectX::XMFLOAT3 pos, bool loop)
{
	//Check if the enum will fit as an index
	if (soundEnum < this->m_sounds3D.size() && soundEnum != Sounds3D::NO_SOUND3D)
	{
		irrklang::ISoundSource* sp = this->m_sounds3D.at(soundEnum);
		irrklang::vec3d<float> pos(pos.x, pos.y, pos.z);
		irrklang::ISound* newActiveSound = this->m_soundEngine->play3D(sp, pos, loop, false, true);
		
		newActiveSound->grab();
		this->m_activeSounds.push_back(newActiveSound);
		
		return 1;
	}

	return 0;

}

void SoundHandler::UpdateSoundHandler()
{
	//Remove sounds that has finnished playing
	std::list<irrklang::ISound*>::iterator itrS;
	for (itrS = this->m_activeSounds.begin(); itrS != this->m_activeSounds.end(); itrS++)
	{
		if((*itrS)->isFinished())
		{
			(*itrS)->drop();
			itrS = this->m_activeSounds.erase(itrS);
		}
	}

	//Check 2D components
#pragma region
	std::vector<SoundComponent2D*>::iterator itr;
	for (itr = this->sound2DComponents.begin(); itr != this->sound2DComponents.end(); itr++) 
	{
		if (!(*itr)->isActive)
		{
			delete (*itr);
			itr = this->sound2DComponents.erase(itr);
		}
		else
		{
			if ((*itr)->sound != Sounds2D::NO_SOUND2D)
			{
				//Play the sound
				this->PlaySound2D((*itr)->sound, (*itr)->loop);
				//Reset the comopnent to play no sound
				(*itr)->sound = Sounds2D::NO_SOUND2D;
			 }
		}
	}
#pragma endregion 2D_COMPONENT_UPDATE
	//Check 3D components
#pragma region
	std::vector<SoundComponent3D*>::iterator itr3;
	for (itr3 = this->sound3DComponents.begin(); itr3 != this->sound3DComponents.end(); itr++)
	{
		if (!(*itr3)->isActive)
		{
			delete (*itr3);
			itr3 = this->sound3DComponents.erase(itr3);
		}
		else
		{
			if ((*itr3)->sound != Sounds3D::NO_SOUND3D)
			{
				//Play the sound
				this->PlaySound3D( (*itr3)->sound, (*itr3)->pos, (*itr3)->loop);
				//Reset the comopnent to play no sound
				(*itr3)->sound = Sounds3D::NO_SOUND3D;
			}
		}
	}

#pragma endregion 3D_COMPONENT_UPDATER
}

SoundComponent2D * SoundHandler::GetSoundComponent2D()
{
	SoundComponent2D* scp = new SoundComponent2D();
	this->sound2DComponents.push_back(scp);
	
	return scp;
}

SoundComponent3D * SoundHandler::GetSoundComponent3D()
{
	SoundComponent3D* scp = new SoundComponent3D();
	this->sound3DComponents.push_back(scp);

	return scp;
}
