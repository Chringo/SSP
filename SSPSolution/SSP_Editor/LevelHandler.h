#ifndef SSPEDITOR_LEVELHANDLER_LEVELHANDLER_H
#define SSPEDITOR_LEVELHANDLER_LEVELHANDLER_H
#include <fstream>
#include "LevelHeaders.h"
#include "Level.h"
class LevelHandler
{
private:
		std::string filePath;
public:
	LevelHandler();
	~LevelHandler();

	void SetFilePath(std::string& filePath) { this->filePath = filePath; };
	LevelData::LevelStatus ExportLevelFile(std::string& filepath);
	LevelData::LevelStatus ImportLevelFile(std::string& filepath);
};

#endif