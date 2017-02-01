#ifndef SSPAPPLICATION_SOUND_SOUNDHANDLER_H
#define	SSPAPPLICATION_SOUND_SOUNDHANDLER_H

#include <vector>
#include <list>
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
	MENU1_3D,
};

struct SoundComponent2D
{
	Sounds2D			sound;
	bool				loop;
	bool				isActive;
};

struct SoundComponent3D
{
	Sounds3D			sound;
	bool				loop;
	bool				isActive;
	DirectX::XMFLOAT3	pos;
};

class SoundHandler : public irrklang::ISoundStopEventReceiver
{
private:
	irrklang::ISoundEngine* m_soundEngine;
	std::vector<SoundComponent2D*> sound2DComponents;
	std::vector<SoundComponent3D*> sound3DComponents;
	std::vector<irrklang::ISoundSource*> m_sounds2D;
	std::vector<irrklang::ISoundSource*> m_sounds3D;
	std::list<irrklang::ISound*> m_activeSounds;
	
	void LoadSounds();

public:
	SoundHandler();
	~SoundHandler();

	int Initialize();
	void Shutdown();

	int PlaySound2D(Sounds2D soundEnum, bool loop);
	int PlaySound3D(Sounds3D soundEnum, DirectX::XMFLOAT3 pos, bool loop);
	
	void UpdateSoundHandler();
	void UpdateListnerPos(DirectX::XMFLOAT3 newPos, DirectX::XMFLOAT3 newLookDir, DirectX::XMFLOAT3 newUpVector);
	SoundComponent2D* GetSoundComponent2D();
	SoundComponent3D* GetSoundComponent3D();

	void OnSoundStopped(irrklang::ISound * sound, irrklang::E_STOP_EVENT_CAUSE reason, void * userData);

};




#endif


