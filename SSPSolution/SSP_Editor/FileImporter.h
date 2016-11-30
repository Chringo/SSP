#ifndef SSPEDITOR_FILEIMPORTER_FILEIMPORTER_H
#define SSPEDITOR_FILEIMPORTER_FILEIMPORTER_H
#include <Windows.h>
#include <vector>
#include <string>
#include "dirent.h"
class FileImporter
{
public:
	FileImporter();
	~FileImporter();

	void ImportFromServer();
private:
	std::vector<std::string> m_filepaths;
};
#endif
