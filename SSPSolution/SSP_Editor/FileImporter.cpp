#include "FileImporter.h"



FileImporter::FileImporter(QTreeWidget *itemList)
{
	m_data = DataHandler::GetInstance();
	this->m_itemList = itemList;
	m_fileLoader = Resources::FileLoader::GetInstance();
}


FileImporter::~FileImporter()
{
}

Resources::Status FileImporter::ImportFromServer()
{
	Ui::AssetTreeHandler* uiTree = Ui::UiControlHandler::GetInstance()->GetAssetTreeController();
	DIR *dir;
	struct dirent *ent;
	QString dirPath = pathToBbfFolder + "/Models";
	int numModels = 0;
	if ((dir = opendir(dirPath.toStdString().c_str())) != NULL)
	{
		/* append all the mesh names from the directory */
		while ((ent = readdir(dir)) != NULL) 
		{
			if (*ent->d_name != '.')
			{
				std::string pathName = dirPath.toStdString() + "/";
				pathName += ent->d_name;
				uiTree->AddItem(
					Ui::AssetTreeHandler::AssetCategories::MODELS, ent->d_name,QVariant(numModels));
				numModels += 1;
				m_filepaths.push_back(pathName);
			}
		}
		closedir(dir);
	}
	else 
	{
		return Resources::Status::ST_ERROR_OPENING_FILE;
		/* could not open directory */
		perror("");
	}

	dirPath = pathToBbfFolder + "/Meshes";
	if ((dir = opendir(dirPath.toStdString().c_str())) != NULL)
	{
		/* append all the mesh names from the directory */
		while ((ent = readdir(dir)) != NULL)
		{
			if (*ent->d_name != '.')
			{
				std::string pathName = dirPath.toStdString() + "/";
				pathName += ent->d_name;
				m_filepaths.push_back(pathName);
			}
		}
		closedir(dir);
	}
	else
	{
		return Resources::Status::ST_ERROR_OPENING_FILE;
		/* could not open directory */
		perror("");
	}
	//if ((dir = opendir("//DESKTOP-BOKNO6D/server/Assets/bbf files/Textures")) != NULL) 
	//{
	//	/* append all the texture names from the directory */
	//	while ((ent = readdir(dir)) != NULL) 
	//	{
	//		if (*ent->d_name != '.')
	//			m_filepaths.push_back(ent->d_name);
	//	}
	//	closedir(dir);
	//}
	//else 
	//{
	//	/* could not open directory */
	//	perror("");
	//}
	dirPath = pathToBbfFolder + "/Materials";
	// Load textures before Materials, So that the materials can find them.
	if ((dir = opendir(dirPath.toStdString().c_str())) != NULL)
	{
		/* append all the mesh names from the directory */
		while ((ent = readdir(dir)) != NULL)
		{
			if (*ent->d_name != '.')
			{
				std::string pathName = dirPath.toStdString() + "/";
				pathName += ent->d_name;
				//AddListItem(ListItem::MATERIAL, ent->d_name);
				m_filepaths.push_back(pathName);
			}
		}
		closedir(dir);
	}
	else
	{
		return Resources::Status::ST_ERROR_OPENING_FILE;
		/* could not open directory */
		perror("");
	}
}

void FileImporter::LoadImportedFiles()
{
	char *m_bbf_object;
	size_t length;
	for (int i = 0; i < m_filepaths.size(); ++i)
	{
		Resources::Status res = m_fileLoader->LoadFile(m_filepaths.at(i), m_bbf_object, &length);

		if (res != Resources::Status::ST_ERROR_OPENING_FILE)
		{
			char *type = m_bbf_object + sizeof(unsigned int);
			Resources::ResourceType loadedObject = (Resources::ResourceType)(*type);

			switch (loadedObject)
			{
			case Resources::ResourceType::RES_MODEL:
				handleModel(m_bbf_object);
				break;
			case Resources::ResourceType::RES_MESH:
				handleMesh(m_bbf_object); //also send integer for the index so we can add the qt
				break;
			case Resources::ResourceType::RES_ANIMATION:
				break;
			case Resources::ResourceType::RES_SKELETON:
				break;
			case Resources::ResourceType::RES_TEXTURE:
				break;
			case Resources::ResourceType::RES_MATERIAL:
				handleMat(m_bbf_object);
				break;
			case Resources::ResourceType::RES_SOUND:
				break;

				//RES_UI

			default:
				break;
			}
		}
	}
}
Resources::Status FileImporter::Initialize()
{
	bool import = false;
	do {
		if (ImportFromServer() == Resources::Status::ST_ERROR_OPENING_FILE) //Try import from server
		{
			if (HandlePathNotFound()){   // If it didnt work, prompt for new path
				SelectNewPath();		// If user wants new path
			}
			else {
				//TODO: Load from BPF file instead
				return Resources::Status::ST_ERROR_OPENING_FILE;
			}
		}
		else
			import = true;
	} while (!import);
	LoadImportedFiles();

	return Resources::Status::ST_OK;
}
void FileImporter::handleMesh(char * m_bbf_object)
{
	/*create model type here and then when reading */
	Resources::Status res;
	Resources::Resource::RawResourceData *res_Data = (Resources::Resource::RawResourceData*)m_bbf_object;

	if (DataHandler::GetInstance()->IDExists(res_Data->m_id))
		return;

	MeshHeader *m_meshH = (MeshHeader*)(m_bbf_object + sizeof(MainHeader));

	Resources::Mesh *newMesh = new Resources::Mesh(*res_Data); //memory handled in Datahandler


	unsigned int * indices;
	BoundingBoxHeader* obbdataPtr;
	if (m_meshH->skeleton)
	{
		Resources::Mesh::VertexAnim* vertices = (Resources::Mesh::VertexAnim*)((char*)m_meshH + sizeof(MeshHeader));
		newMesh->SetVertices(vertices, this->m_Device, m_meshH->numVerts, true);
		indices = (unsigned int*)((char*)vertices + (sizeof(Resources::Mesh::VertexAnim)* m_meshH->numVerts));
		obbdataPtr = (BoundingBoxHeader*)((char*)indices + sizeof(unsigned int) * m_meshH->indexLength);
	}
	else
	{
		Resources::Mesh::Vertex* vertices = (Resources::Mesh::Vertex*)((char*)m_meshH + sizeof(MeshHeader));
		newMesh->SetVertices(vertices, this->m_Device, m_meshH->numVerts, true);
		indices = (unsigned int*)((char*)vertices + (sizeof(Resources::Mesh::Vertex)* m_meshH->numVerts));
		obbdataPtr = (BoundingBoxHeader*)((char*)indices + sizeof(unsigned int) * m_meshH->indexLength);
	}

	if (!newMesh->SetIndices(indices, m_meshH->indexLength, this->m_Device, true))
		res = Resources::Status::ST_BUFFER_ERROR;

	
	BoundingBoxHeader obbdata = *obbdataPtr;
	m_data->AddMesh(newMesh);
	std::vector<Resources::Model*>* models = m_data->GetModels();
	
	for (int i = 0; i < models->size(); ++i)
	{
		if (models->at(i)->GetRawModelData()->meshId == newMesh->GetId())
		{
			models->at(i)->SetMesh(newMesh);
			models->at(i)->SetOBBData(obbdata);
		}
	}
	/*add to the ui here*/

	//Resources::Model *importedModel = new Resources::Model;
}

void FileImporter::handleMat(char * m_bbf_object)
{
	Resources::Status stat;
	Resources::Resource::RawResourceData * res_Data = (Resources::Resource::RawResourceData*)m_bbf_object;

	if (DataHandler::GetInstance()->IDExists(res_Data->m_id))
		return;

	MaterialHeader* m_MatH = (MaterialHeader*)(m_bbf_object + sizeof(MainHeader));

	Resources::Material* newMaterial = new Resources::Material(*res_Data);
	
	Resources::Texture *test = m_data->GetTextureHandler()->GetPlaceHolderTextures();  // TEMPORARY
																					   // TEMPORARY
	newMaterial->SetTexture(&test[0], Resources::TEXTURE_ALBEDO);					   // TEMPORARY
	newMaterial->SetTexture(&test[1], Resources::TEXTURE_SPECULAR);					   // TEMPORARY
	newMaterial->SetTexture(&test[2], Resources::TEXTURE_ROUGHNESS);				   // TEMPORARY
	newMaterial->SetTexture(&test[3], Resources::TEXTURE_NORMAL);					   // TEMPORARY
	newMaterial->SetTexture(&test[4], Resources::TEXTURE_AO);						   // TEMPORARY

	newMaterial->SetMetallic(m_MatH->m_Metallic);
	newMaterial->SetRoughness(m_MatH->m_Roughness);
	newMaterial->SetEmissive(m_MatH->m_EmissiveValue);

	m_data->AddMaterial(newMaterial); //add the material to the data handler

	std::vector<Resources::Model*>* models = m_data->GetModels();
	for (int i = 0; i < models->size(); ++i)
	{
		if (models->at(i)->GetRawModelData()->materialId == newMaterial->GetId())
		{
			models->at(i)->SetMaterial(newMaterial);
		}
	}
}

void FileImporter::handleModel(char * m_bbf_object)
{
	
	Resources::Status res;
	Resources::Resource::RawResourceData *res_Data = (Resources::Resource::RawResourceData*)m_bbf_object;
	if (m_data->IDExists(res_Data->m_id))
		return;
	Resources::Model::RawModelData *raw_model_Data = (Resources::Model::RawModelData*)(m_bbf_object + sizeof(MainHeader));

	Resources::Model *newModel = new Resources::Model();

	newModel->Create(res_Data, raw_model_Data, true);
	m_data->AddModel(newModel);
	
}

void FileImporter::AddListItem(ListItem category, std::string name)
{
	QTreeWidgetItem *itm = new QTreeWidgetItem();
	
	itm->setText(0, name.substr(0, name.rfind(".")).c_str());
	this->m_itemList->topLevelItem((int)category)->addChild(itm);

}

bool FileImporter::HandlePathNotFound()
{


	QMessageBox msgBox;
	msgBox.setText("Could not find assets folder");

	msgBox.setInformativeText("Would you like to browse for assets?");
	msgBox.setStandardButtons(QMessageBox::Yes |  QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret) {
	case QMessageBox::Yes:
		return true;
	case QMessageBox::No:
		return false;
	}

	return false;
}

std::string FileImporter::SelectNewPath()
{

	QMessageBox msgBox;
	msgBox.setText("Please locate the folder \"bbf files\"");
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.setDefaultButton(QMessageBox::Ok);
	msgBox.exec();
	QFileDialog dlg(NULL, "Load file");
	dlg.setAcceptMode(QFileDialog::AcceptOpen);
	//QFileDialog::ReadOnly
	dlg.setNameFilter("bbf files");
	dlg.setObjectName("bbf files");
	dlg.setOption(QFileDialog::Option::ShowDirsOnly, true);
	QString  newPath = dlg.getExistingDirectory(nullptr, "Open Directory",
		"/bbf files",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);

	
	if (newPath.length() > 2)
	{
		this->pathToBbfFolder = newPath;
		return newPath.toStdString(); //Return file path as string
	}
	else
	{
		return "";
	}

	
}
