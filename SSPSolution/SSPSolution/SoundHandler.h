#ifndef SSPAPPLICATION_SOUND_SOUNDHANDLER_H
#define	SSPAPPLICATION_SOUND_SOUNDHANDLER_H

#include "../irrKlang/Include/irrKlang.h"
#pragma comment (lib, "../irrKlang/lib/irrKlang")

class SoundHandler
{
private:
	irrklang::ISoundEngine* soundEngine;


public:
	SoundHandler();
	~SoundHandler();

	int Initialize();
	void Shutdown();

};




#endif


