#ifndef SSPEDITOR_FILEIMPORTER_FILEIMPORTER_H
#define SSPEDITOR_FILEIMPORTER_FILEIMPORTER_H
#include <Windows.h>
#include <vector>
#include <string>
#include "dirent.h"
#include "../ResourceLib/FileLoader.h"
#include "../ResourceLib/Model.h"
#include "../ResourceLib/FileHeaders.h"
#include <QtWidgets\qtreewidget.h>
class FileImporter
{
private:
	enum ListItem
	{
		MODEL = 0,
		ANIM  = 1,
	};

	/*variables*/
	std::vector<std::string> m_filepaths;
	Resources::FileLoader *m_fileLoader;

	//put model vector hetrer;
	std::vector<Resources::Model*> m_models;
	QTreeWidget *m_itemList;

public:
	FileImporter(QTreeWidget *itemList);
	~FileImporter();

	void ImportFromServer();
	void LoadImportedFiles();

	std::vector<Resources::Model*>* get_M_models() { return &this->m_models; }
private:
	/*functions*/
	void handleMesh(char * m_bbf_object);
	void handleMat(char *m_bbf_object);
	void handleModel(char *m_bbf_object);

	void AddListItem(ListItem category, std::string name);
};
#endif
