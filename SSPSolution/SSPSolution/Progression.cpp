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

}

bool Progression::WriteToFile(std::string filename)
{
	std::ofstream saveFile;

	saveFile.open("..\\Debug\\Saves\\" + filename + ".txt");

	if (!saveFile.is_open()) 
	{
		return false;
	}
	else
	{
		saveFile << this->m_currentLevel << "\r\n";
		saveFile << this->m_currentCheckpoint << "\r\n";
		saveFile << this->m_unlockedLevels << "\r\n";
		saveFile.close();
	}
	return true;
}

bool Progression::ReadFromFile(std::string filename)
{
	std::ifstream loadFile;
	std::string path = "..\\Debug\\Saves\\" + filename + ".txt";
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
