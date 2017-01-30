#ifndef SSPAPPLICATION_SOUND_SOUNDHANDLER_H
#define	SSPAPPLICATION_SOUND_SOUNDHANDLER_H

#include <vector>
#include <DirectXMath.h>
#include "../irrKlang/Include/irrKlang.h"
#pragma comment (lib, "../irrKlang/lib/irrKlang")

enum Sounds2D
{
	NO_SOUND2D = -1,
	SOUND1,
	SOUND2,
};

enum Sounds3D
{
	NO_SOUND3D = -1,
};

struct SoundComponent
{
	unsigned int		entityID;
	Sounds3D			sound;
	bool				loop;
	bool				isActive;
	DirectX::XMFLOAT3	pos;

};


class SoundHandler
{
private:
	irrklang::ISoundEngine* m_soundEngine;
	std::vector<SoundComponent*> soundComponents;
	std::vector<irrklang::ISoundSource*> m_sounds2D;
	std::vector<irrklang::ISoundSource*> m_sounds3D;
	
	void LoadSounds();

public:


	SoundHandler();
	~SoundHandler();

	int Initialize();
	void Shutdown();

	int PlaySound2D(Sounds2D soundEnum);
	int PlaySound3D(Sounds3D soundEnum, DirectX::XMFLOAT3 pos);
	
	void UpdateSoundHandler();
	SoundComponent* GetSoundComponent();

};




#endif


