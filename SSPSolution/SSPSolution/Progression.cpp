#include "Progression.h"

Progression* Progression::m_instance = nullptr;

Progression::Progression()
{

}

Progression::~Progression()
{

}

bool Progression::WriteToFile(std::string filename)
{
	std::ofstream saveFile;

	saveFile.open("..\\Debug\\Saves\\" + filename + ".txt");

	if (!saveFile.is_open()) {
		return false;
	}
	else
	{

		saveFile << "Allhuakbar" << "\r\n";
		saveFile.close();
	}
	return true;
}

bool Progression::ReadFromFile(std::string filename)
{
	return false;
}
