#ifndef SSPAPPLICATION_SOUND_SOUNDHANDLER_H
#define	SSPAPPLICATION_SOUND_SOUNDHANDLER_H

#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <unordered_map>
#include <DirectXMath.h>
#include "../irrKlang/Include/irrKlang.h"
#pragma comment (lib, "../irrKlang/lib/irrKlang")


enum Sounds2D
{
	NO_SOUND2D = -1,
	MENU1,
	MENU2,
};

enum Sounds3D
{
	NO_SOUND3D = -1,
	ABBINGTON_FLYING_1,
	ABBINGTON_FLYING_2,
	ABBINGTON_FLYING_3,
	ABBINGTON_LANDING,
	ABBINGTON_THROWING_1,
	ABBINGTON_THROWING_2,
	ABBINGTON_THROWING_3,
	ABBINGTON_WALK,
	STUDLEY_FLYING_1,
	STUDLEY_FLYING_2,
	STUDLEY_FLYING_3,
	STUDLEY_LANDING,
	STUDLEY_THROW_1,
	STUDLEY_THROW_2,
	STUDLEY_THROW_3,
	STUDLEY_WALK,
	GENERAL_ACTIVATE,
	GENERAL_BUTTON_CLICKED,
	GENERAL_CHAIN_DRAG_1,
	GENERAL_CHAIN_DRAG_2,
	GENERAL_CHAIN_DRAG_3,
	GENERAL_CHAIN_HIGHER_1,
	GENERAL_CHAIN_HIGHER_2,
	GENERAL_CHAIN_HIGHER_3,
	GENERAL_CHAINBALL_DROPPED,
	GENERAL_LEVER,
	GENERAL_LIFT,
	GENERAL_DOOR_OPENING,


};

class SoundHandler : public irrklang::ISoundStopEventReceiver
{
private:
	irrklang::ISoundEngine* m_soundEngine;
	std::vector<irrklang::ISoundSource*> m_sounds2D;
	std::vector<irrklang::ISoundSource*> m_sounds3D;

	unsigned m_randomSeed;

	void LoadSounds();
	void DropSounds();	//Drop irrKlang resources that is keept outside of the Sound engine

public:
	SoundHandler();
	~SoundHandler();

	int Initialize();
	void Shutdown();

	irrklang::ISound* PlaySound2D(Sounds2D soundEnum, bool loop, bool track);
	irrklang::ISound* PlaySound3D(Sounds3D soundEnum, DirectX::XMFLOAT3 pos, bool loop, bool track);
	
	irrklang::ISound* PlayRandomSound2D(Sounds2D start_soundEnum, Sounds2D end_soundEnum, bool loop, bool track);
	irrklang::ISound* PlayRandomSound3D(Sounds3D start_soundEnum, Sounds3D end_soundEnum, DirectX::XMFLOAT3 pos, bool loop, bool track);



	void UpdateListnerPos(DirectX::XMFLOAT3 newPos, DirectX::XMFLOAT3 newLookDir, DirectX::XMFLOAT3 newUpVector);

	void OnSoundStopped(irrklang::ISound * sound, irrklang::E_STOP_EVENT_CAUSE reason, void * userData);
	bool ReInitSoundEngine();	//Try to reload the irrKlang engine and loaded sounds


};




#endif


