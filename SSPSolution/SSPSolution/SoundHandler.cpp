#include "SoundHandler.h"

SoundHandler* SoundHandler::m_instance = nullptr;

SoundHandler::~SoundHandler()
{
}

int SoundHandler::Initialize()
{

	this->m_soundEngine = irrklang::createIrrKlangDevice();
	//Time is defined as __int64, data loss might occur from limiting the value to an unsigned int
	this->m_randomSeed = unsigned int(time(NULL));	//Set the seed
	srand(this->m_randomSeed);
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

	delete this->m_instance;

}

void SoundHandler::LoadSounds()
{
	irrklang::ISoundSource* sp = nullptr;

	//Load 2D sounds
#pragma region
	//Menu1
	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Level_music.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultVolume(0.1f);
		this->m_sounds2D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	//Menu2
	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Menu_Music_stereo2.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultVolume(1.f);
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
	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Abbington_flying1.mp3");

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
	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Abbington_flying2.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}
	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Abbington_flying3.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}
	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Abbington_landing.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}
	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Abbington_throw1_mono.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Abbington_throw2_mono.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}
	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Abbington_throw3_mono.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}
	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Abbington_Walk_mono.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}
	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Studley_flying1_mono.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}
	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Studley_flying2_mono.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}
	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Studley_flying3_mono.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}
	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Studley_landing_mono.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}
	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Studley_throw1_mono.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}
	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Studley_throw2_mono.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Studley_throw3_mono.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Studley_walk_mono.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}
	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/General_Activate_mono.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/General_Button_Clicked.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}
	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/General_chain_drag_mono1.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		sp->setDefaultVolume(0.3f);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/General_chain_drag_mono2.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		sp->setDefaultVolume(0.3f);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/General_chain_drag_mono3.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		sp->setDefaultVolume(0.3f);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/General_chain_higher_mono1.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		sp->setDefaultVolume(1.f);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/General_chain_higher_mono2.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/General_chain_higher_mono3.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/General_Chainball_dropped_mono1.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/General_Lever_mono.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/General_Lift_move.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/Generel_Door_opening_mono.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/General_Wheel.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/General_Wheel_Clicked.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/General_Button_Timer.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		sp->setDefaultVolume(0.8f);
		this->m_sounds3D.push_back(sp);
	}
	else
	{
		printf("Failed to load sound");
	}

	sp = m_soundEngine->addSoundSourceFromFile("../Assets/Sounds/General_PingSound.mp3");

	if (sp != nullptr)
	{
		sp->grab();
		sp->setDefaultMinDistance(SOUND_MIN_DISTANCE);
		sp->setDefaultVolume(1.3f);
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
	if (soundEnum < (int)this->m_sounds2D.size() && soundEnum != Sounds2D::NO_SOUND2D)
	{
		irrklang::ISoundSource* sp = this->m_sounds2D.at(soundEnum);
		irrklang::ISound* newActiveSound = this->m_soundEngine->play2D(sp, loop, false, true);

		if (newActiveSound)	//If the sound is created/playing correctly
		{
			newActiveSound->grab();

			if (track == false)	//If we want the soundEngine to handle cleanup
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
	if (soundEnum < (int)this->m_sounds3D.size() && soundEnum != Sounds3D::NO_SOUND3D)
	{
		irrklang::ISoundSource* sp = this->m_sounds3D.at(soundEnum);
		irrklang::vec3d<float> pos(pos.x, pos.y, pos.z);
		irrklang::ISound* newActiveSound = this->m_soundEngine->play3D(sp, pos, loop, false, true);
		
		if (newActiveSound)	//If the sound is created/playing correctly
		{
			newActiveSound->grab();
			
			if (track == false)	//If we want the soundEngine to handle cleanup
			{
				newActiveSound->setSoundStopEventReceiver(this);
				return nullptr;
			}
			
			return newActiveSound;
		}
		
	}

	return nullptr;
}

irrklang::ISound * SoundHandler::PlayRandomSound2D(Sounds2D start_soundEnum, Sounds2D end_soundEnum, bool loop, bool track)
{
	//Check if the enum will fit as an index
	if (start_soundEnum < (int)this->m_sounds2D.size() && start_soundEnum != Sounds2D::NO_SOUND2D &&
		end_soundEnum < (int)this->m_sounds2D.size() && end_soundEnum != Sounds2D::NO_SOUND2D)
	{

		int randomSoundID = (rand() % ((end_soundEnum + 1) - start_soundEnum)) + start_soundEnum;	//Get a random sound between the defined sounds

		irrklang::ISoundSource* sp = this->m_sounds2D.at(randomSoundID);
		irrklang::ISound* newActiveSound = this->m_soundEngine->play2D(sp, loop, false, true);

		if (newActiveSound)	//If the sound is created/playing correctly
		{
			newActiveSound->grab();

			if (track == false)	//If we want the soundEngine to handle cleanup
			{
				newActiveSound->setSoundStopEventReceiver(this);
				return nullptr;
			}

			return newActiveSound;
		}

	}

	return nullptr;
}

irrklang::ISound * SoundHandler::PlayRandomSound3D(Sounds3D start_soundEnum, Sounds3D end_soundEnum, DirectX::XMFLOAT3 pos, bool loop, bool track)
{
	//Check if the enum will fit as an index
	if (start_soundEnum < (int)this->m_sounds3D.size() && start_soundEnum != Sounds3D::NO_SOUND3D &&
		end_soundEnum < (int)this->m_sounds3D.size() && end_soundEnum != Sounds3D::NO_SOUND3D)
	{
		int randomSoundID = (rand() % ((end_soundEnum + 1) - start_soundEnum)) + start_soundEnum;	//Get a random sound between the defined sounds

		irrklang::ISoundSource* sp = this->m_sounds3D.at(randomSoundID);
		irrklang::vec3d<float> pos(pos.x, pos.y, pos.z);
		irrklang::ISound* newActiveSound = this->m_soundEngine->play3D(sp, pos, loop, false, true);

		if (newActiveSound)	//If the sound is created/playing correctly
		{
			newActiveSound->grab();

			if (track == false)	//If we want the soundEngine to handle cleanup
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
		result = this->Initialize() != 0;
	}

	return result;
}
