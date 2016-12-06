#ifndef SSPEDITOR_FILEIMPORTER_FILEIMPORTER_H
#define SSPEDITOR_FILEIMPORTER_FILEIMPORTER_H
#include <Windows.h>
#include <vector>
#include <string>
#include "dirent.h"
#include "../ResourceLib/FileLoader.h"
#include "../ResourceLib/Model.h"
#include "../ResourceLib/FileHeaders.h"
class FileImporter
{
private:
	/*variables*/
	std::vector<std::string> m_filepaths;
	Resources::FileLoader *m_fileLoader;

	//put model vector hetrer;
	std::vector<Resources::Model*> m_models;

public:
	FileImporter();
	~FileImporter();

	void ImportFromServer();
	void LoadImportedFiles();
private:
	/*functions*/
	void handleMesh(char * m_bbf_object);
	void handleMat(char *m_bbf_object);
};
#endif
