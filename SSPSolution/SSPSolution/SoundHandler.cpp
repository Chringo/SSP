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
		this->m_soundEngine = nullptr;
		return 0;	//Error on creation
	}

	this->LoadSounds();

	return 1;
}

void SoundHandler::Shutdown()
{
	this->DropSounds();	//Drop all irrKlang related resourcses

	if (this->m_soundEngine)
	{
		this->m_soundEngine->drop();
	}

}

void SoundHandler::LoadSounds()
{
	irrklang::ISoundSource* sp = nullptr;

	//Load 2D sounds
#pragma region
	//Menu1
	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/Menu_Music_stereo1.mp3");
	if (sp != nullptr)
	{
		sp->grab();
		this->m_sounds2D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	//Menu2
	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/Menu_Music_stereo2.mp3");
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
	//Menu1 for 3D
	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/Abbington_flying1.mp3");
	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(20);	//At what distance the sound cant be heard (not sure on the units)
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	//Menu1 for 3D
	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/Abbington_flying2.mp3");
	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(20);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/Abbington_flying3.mp3");
	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(20);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/Abbington_landing.mp3");
	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(20);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/Abbington_throw1_mono.mp3");
	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(20);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/Abbington_throw2_mono.mp3");
	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(20);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/Abbington_throw3_mono.mp3");
	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(20);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/Abbington_Walk_mono.mp3");
	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(20);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/Studley_flying1_mono.mp3");
	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(20);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/Studley_flying2_mono.mp3");
	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(20);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/Studley_flying3_mono.mp3");
	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(20);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/Studley_landing_mono.mp3");
	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(20);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/Studley_throw1_mono.mp3");
	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(20);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/Studley_throw2_mono.mp3");
	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(20);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/Studley_throw3_mono.mp3");
	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(20);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Debug/Sounds/Studley_walk_mono.mp3");
	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(20);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

#pragma endregion 3D_Sounds
	
	sp = nullptr;

}

void SoundHandler::DropSounds()
{
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
}

irrklang::ISound* SoundHandler::PlaySound2D(Sounds2D soundEnum, bool loop, bool track)
{
	//Check if the enum will fit as an index
	if (soundEnum < this->m_sounds2D.size() && soundEnum != Sounds2D::NO_SOUND2D)
	{
		irrklang::ISoundSource* sp = this->m_sounds2D.at(soundEnum);
		irrklang::ISound* newActiveSound = this->m_soundEngine->play2D(sp, loop, false, true);

		if (newActiveSound)	//If the sound is created/playing correctly
		{
			newActiveSound->grab();

			if (track == true)	//If we want the soundEngine to handle cleanup
			{
				newActiveSound->setSoundStopEventReceiver(this);
				return nullptr;
			}

			return newActiveSound;
		}

	}

	return nullptr;
}

irrklang::ISound* SoundHandler::PlaySound3D(Sounds3D soundEnum, DirectX::XMFLOAT3 pos, bool loop, bool track)
{
	//Check if the enum will fit as an index
	if (soundEnum < this->m_sounds3D.size() && soundEnum != Sounds3D::NO_SOUND3D)
	{
		irrklang::ISoundSource* sp = this->m_sounds3D.at(soundEnum);
		irrklang::vec3d<float> pos(pos.x, pos.y, pos.z);
		irrklang::ISound* newActiveSound = this->m_soundEngine->play3D(sp, pos, loop, false, true);
		
		if (newActiveSound)	//If the sound is created/playing correctly
		{
			newActiveSound->grab();
			
			if (track == true)	//If we want the soundEngine to handle cleanup
			{
				newActiveSound->setSoundStopEventReceiver(this);
				return nullptr;
			}
			
			return newActiveSound;
		}
		
	}

	return nullptr;
}

void SoundHandler::UpdateListnerPos(DirectX::XMFLOAT3 newPos, DirectX::XMFLOAT3 newLookDir, DirectX::XMFLOAT3 newUpVector)
{
	if (this->m_soundEngine)
	{
		irrklang::vec3df position(newPos.x, newPos.y, newPos.z);        // position of the listener
		irrklang::vec3df lookDirection(newLookDir.x, newLookDir.y, newLookDir.z); // the direction the listener looks into
		irrklang::vec3df velPerSecond(0, 0, 0);    // only relevant for doppler effects
		irrklang::vec3df upVector(newUpVector.x, newUpVector.y, newUpVector.z);        // where 'up' is in your 3D scene

		this->m_soundEngine->setListenerPosition(position, lookDirection, velPerSecond, upVector);
	}
}

void SoundHandler::OnSoundStopped(irrklang::ISound * sound, irrklang::E_STOP_EVENT_CAUSE reason, void * userData)
{
	//Remove sounds that has finnished playing
	sound->drop();

}

bool SoundHandler::ReInitSoundEngine()
{
	bool result = false;
	irrklang::ISoundDeviceList* deviceList = irrklang::createSoundDeviceList();
	//If there is a sound device to use
	if (deviceList->getDeviceCount() != 0)
	{	
		this->DropSounds();	//Drop all loaded sound since they must be cnnected with the new engine
		result = this->Initialize();
	}

	return result;
}
