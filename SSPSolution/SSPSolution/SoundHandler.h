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
};

struct SoundComponent2D
{
	Sounds2D			sound;
	bool				loop;
	bool				isActive;

};


class SoundHandler
{
private:
	irrklang::ISoundEngine* m_soundEngine;
	std::vector<SoundComponent2D*> sound2DComponents;
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
	SoundComponent2D* GetSoundComponent2D();

};




#endif


