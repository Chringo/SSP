#ifndef SSPAPPLICATION_CORE_PROGRESSION_H
#define SSPAPPLICATION_CORE_PROGRESSION_H

#include <fstream>
#include <string>
#include <DirectXMath.h>

/* Assuming that the puzzele will be reloaded with a 
predefined state so we dont have to store entity 
states and positions. */

class Progression
{

private:

	static Progression* m_instance;

	// CURRENT LEVEL DATA 
	unsigned int m_currentLevel;		// Assuming that each puzzele will have an ID of some sort from 0 to w/e
	unsigned int m_currentCheckpoint;	// Assuming that each puzzele MIGHT have more than one checkpoint where they have different IDs
	
	// UNLOCK DATA
	unsigned int m_unlockedLevels;		// How many levels are unlocked

	// IP DATA
	std::wstring m_ipString;

	Progression();

public:
	~Progression();

	static Progression& instance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new Progression();
		}
		return *m_instance;
	}

	bool WriteToFile(std::string filename);
	bool ReadFromFile(std::string filename);

	unsigned int GetCurrentLevel();
	unsigned int GetCurrentCheckpoint();
	unsigned int GetUnlockedLevels();
	std::wstring GetIPString();

	void SetCurrentLevel(unsigned int levelID);
	void SetCurrentCheckpoint(unsigned int checkpointID);
	void SetUnlockedLevels(unsigned int newLevels);
	void SetIPString(std::wstring ipString);
};

#endif
