#ifndef SSPEDITOR_LEVELHANDLER_LEVELHANDLER_H
#define SSPEDITOR_LEVELHANDLER_LEVELHANDLER_H
#include <fstream>
#include "LevelHeaders.h"
#include "Level.h"
#include <qfiledialog.h>
#include <DataHandler.h>

class LevelHandler
{
	enum Operation {
		LOAD,
		SAVE
	};
private:
	Level m_currentLevel;
	QString filePath = "//DESKTOP-BOKNO6D/server/Assets/bbf files/Levels/";
	LevelHandler();

public:
	~LevelHandler();
	static LevelHandler* GetInstance();

	LevelData::LevelStatus ExportLevelFile();
	LevelData::LevelStatus ImportLevelFile();
	LevelData::LevelStatus NewLevel();
	Level* GetCurrentLevel() { return &this->m_currentLevel; };
private:
	std::string GetFilePathAndName(Operation flag);
	LevelData::MainLevelHeader GetMainHeader();
 };

#endif