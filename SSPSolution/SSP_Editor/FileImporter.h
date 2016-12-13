#ifndef SSPEDITOR_FILEIMPORTER_FILEIMPORTER_H
#define SSPEDITOR_FILEIMPORTER_FILEIMPORTER_H
#include <Windows.h>
#include <vector>
#include <string>
#include "dirent.h"
#include "../ResourceLib/FileLoader.h"
#include "../ResourceLib/Model.h"
#include "../ResourceLib/FileHeaders.h"
#include "../ResourceLib/TextureHandler.h"
#include <QtWidgets\qtreewidget.h>
class FileImporter
{
private:
	enum ListItem
	{
		MODEL = 0,
		ANIM  = 1,
		MATERIAL = 2,
	};

	/*variables*/
	std::vector<std::string> m_filepaths;
	Resources::FileLoader *m_fileLoader;

	//put model vector hetrer;
	std::vector<Resources::Model*> m_models;
	QTreeWidget *m_itemList;
	ID3D11Device* m_Device;

public:
	FileImporter(QTreeWidget *itemList);
	~FileImporter();

	void ImportFromServer();
	void LoadImportedFiles();
	void setDevice(ID3D11Device* device) { this->m_Device = device; };

	std::vector<Resources::Model*>* get_M_models() { return &this->m_models; }
	Resources::Model* get_model(unsigned int modelID);
private:
	/*functions*/
	void handleMesh(char * m_bbf_object);
	void handleMat(char *m_bbf_object);
	void handleModel(char *m_bbf_object);

	void AddListItem(ListItem category, std::string name);
};
#endif
