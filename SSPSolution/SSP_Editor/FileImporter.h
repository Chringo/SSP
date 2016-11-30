#ifndef SSPEDITOR_FILEIMPORTER_FILEIMPORTER_H
#define SSPEDITOR_FILEIMPORTER_FILEIMPORTER_H
#include <Windows.h>
#include <vector>
#include <string>
#include "dirent.h"
#include "../ResourceLib/FileLoader.h"
class FileImporter
{
public:
	FileImporter();
	~FileImporter();

	void ImportFromServer();
	void LoadImportedFiles();
private:
	std::vector<std::string> m_filepaths;
	Resources::FileLoader *m_fileLoader;
};
#endif
