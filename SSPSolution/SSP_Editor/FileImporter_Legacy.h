#ifdef LEGACY_IMPORT

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
#include "DataHandler.h"
#include "qmessagebox.h"
#include "qfiledialog.h"
#include "UiControlHandler.h"
#include "../ResourceLib/TextureLoader/DDSTextureLoader.h"

class FileImporter
{
private:
	enum ListItem
	{
		MODEL = 0,
		ANIM  = 1,
		MATERIAL = 2,
	};

	enum SpecialImportCases
	{
		NONE = 0,
		PLAYER,
		GENERAL,
		SUBCATEGORIES,
	};

	/*variables*/
	std::vector<std::string> m_filepaths;
	Resources::FileLoader *m_fileLoader;
	DataHandler* m_data;
	
	QTreeWidget *m_itemList;
	ID3D11Device* m_Device;
	QString pathToBbfFolder = "//DESKTOP-BOKNO6D/server/Assets/bbf files";

public:
	FileImporter(QTreeWidget *itemList);
	~FileImporter();

	Resources::Status ImportFromServer();
	Resources::Status Initialize();
	void LoadImportedFiles();
	void setDevice(ID3D11Device* device) { this->m_Device = device; };

	std::vector<Resources::Model*>* get_M_models() { return this->m_data->GetModels(); }
	std::vector<std::string>* GetFilePaths() { return &m_filepaths; }

private:
	/*functions*/
	void handleMesh(char * m_bbf_object);
	void handleMat(char *m_bbf_object);
	void handleModel(char *m_bbf_object);
	void handleSkeleton(char *m_bbf_object);
	void handleAnimation(char *m_bbf_object);

	void AddListItem(ListItem category, std::string name);
	void GeneratePlaceholderMaterial(Resources::Model* m_Model);
	bool ImportTextures(char * m_bbf_object, MaterialHeader* m_Mheader, Resources::Material * newMaterial);
	Resources::Status AppendFromFolder(QString * dirPath, DIR *dir, int & numModels, Ui::AssetTreeHandler* uiTree, SpecialImportCases Simp,
		Ui::AssetTreeHandler::AssetCategories type, Ui::AssetTreeHandler::AssetSubCategories subType);
	

	bool HandlePathNotFound();
	std::string SelectNewPath();
};
#endif
#endif //Legacy FileImport