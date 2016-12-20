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
