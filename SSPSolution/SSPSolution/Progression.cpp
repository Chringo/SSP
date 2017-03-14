#include "Progression.h"

Progression* Progression::m_instance = nullptr;

Progression::Progression()
{
	this->m_currentLevel = 0;
	this->m_currentCheckpoint = 0;
	this->m_unlockedLevels = 0;
}

Progression::~Progression()
{
	this->WriteToFile("save1");
}

bool Progression::WriteToFile(std::string filename)
{
	std::wofstream saveFile;

	saveFile.open("../Assets/Saves/" + filename + ".txt");

	if (!saveFile.is_open()) 
	{
		return false;
	}
	else
	{
		saveFile << this->m_currentLevel << "\r\n";
		saveFile << this->m_currentCheckpoint << "\r\n";
		saveFile << this->m_unlockedLevels << "\r\n";
		saveFile << this->m_ipString << "\r\n";
		saveFile.close();
	}
	return true;
}

bool Progression::ReadFromFile(std::string filename)
{
	std::wifstream loadFile;
	std::string path = "../Assets/Saves/" + filename + ".txt";
	loadFile.open(path);

	if (!loadFile.is_open()) 
	{
		return false;
	}
	else
	{
		loadFile >> this->m_currentLevel;
		loadFile >> this->m_currentCheckpoint;
		loadFile >> this->m_unlockedLevels;
		loadFile >> this->m_ipString;

		loadFile.close();
	}

	return true;
}

unsigned int Progression::GetCurrentLevel()
{
	return this->m_currentLevel;
}

unsigned int Progression::GetCurrentCheckpoint()
{
	return this->m_currentCheckpoint;
}

unsigned int Progression::GetUnlockedLevels()
{
	return this->m_unlockedLevels;
}

std::wstring Progression::GetIPString()
{
	return this->m_ipString;
}

void Progression::SetCurrentLevel(unsigned int levelID)
{
	this->m_currentLevel = levelID;
}

void Progression::SetCurrentCheckpoint(unsigned int checkpointID)
{
	this->m_currentCheckpoint = checkpointID;
}

void Progression::SetUnlockedLevels(unsigned int newLevels)
{
	this->m_unlockedLevels = newLevels;
}

void Progression::SetIPString(std::wstring ipString)
{
	this->m_ipString = ipString;
}
