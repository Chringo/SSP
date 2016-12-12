#include "FileImporter.h"



FileImporter::FileImporter(QTreeWidget *itemList)
{
	this->m_itemList = itemList;
	m_fileLoader = Resources::FileLoader::GetInstance();
}


FileImporter::~FileImporter()
{
	m_models.clear();
}

void FileImporter::ImportFromServer()
{
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir("//DESKTOP-BOKNO6D/server/Assets/bbf files/Models")) != NULL) 
	//if ((dir = opendir("C:/Users/Cool_David_92/Desktop/hehee/Meshes")) != NULL)
	{
		/* append all the mesh names from the directory */
		while ((ent = readdir(dir)) != NULL) 
		{
			if (*ent->d_name != '.')
			{
				std::string pathName = "//DESKTOP-BOKNO6D/server/Assets/bbf files/Models/";
				pathName += ent->d_name;
				AddListItem(ListItem::MODEL, ent->d_name);
				m_filepaths.push_back(pathName);
			}
		}
		closedir(dir);
	}
	else 
	{
		/* could not open directory */
		perror("");
	}
	if ((dir = opendir("//DESKTOP-BOKNO6D/server/Assets/bbf files/Meshes")) != NULL)
		//if ((dir = opendir("C:/Users/Cool_David_92/Desktop/hehee/Meshes")) != NULL)
	{
		/* append all the mesh names from the directory */
		while ((ent = readdir(dir)) != NULL)
		{
			if (*ent->d_name != '.')
			{
				std::string pathName = "//DESKTOP-BOKNO6D/server/Assets/bbf files/Meshes/";
				pathName += ent->d_name;
				m_filepaths.push_back(pathName);
			}
		}
		closedir(dir);
	}
	else
	{
		/* could not open directory */
		perror("");
	}
	if ((dir = opendir("//DESKTOP-BOKNO6D/server/Assets/bbf files/Materials")) != NULL)
		//if ((dir = opendir("C:/Users/Cool_David_92/Desktop/hehee/Meshes")) != NULL)
	{
		/* append all the mesh names from the directory */
		while ((ent = readdir(dir)) != NULL)
		{
			if (*ent->d_name != '.')
			{
				std::string pathName = "//DESKTOP-BOKNO6D/server/Assets/bbf files/Materials/";
				pathName += ent->d_name;
				//AddListItem(ListItem::MATERIAL, ent->d_name);
				m_filepaths.push_back(pathName);
			}
		}
		closedir(dir);
	}
	else
	{
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

Resources::Model * FileImporter::get_model(unsigned int modelID)
{
	for (int i = 0; i < this->m_models.size(); ++i)
	{
		if (modelID == m_models.at(i)->GetId())
			return m_models.at(i);
	}
	return nullptr;
}

void FileImporter::handleMesh(char * m_bbf_object)
{
	/*create model type here and then when reading */
	Resources::Status res;
	Resources::Resource::RawResourceData *res_Data = (Resources::Resource::RawResourceData*)m_bbf_object;

	MeshHeader *m_meshH = (MeshHeader*)(m_bbf_object + sizeof(MainHeader));

	Resources::Mesh *newMesh = new Resources::Mesh(*res_Data);


	unsigned int * indices;
	if (m_meshH->skeleton)
	{
		Resources::Mesh::VertexAnim* vertices = (Resources::Mesh::VertexAnim*)((char*)m_meshH + sizeof(MeshHeader));
		newMesh->SetVertices(vertices, this->m_Device, m_meshH->numVerts, true);
		indices = (unsigned int*)((char*)vertices + (sizeof(Resources::Mesh::VertexAnim)* m_meshH->numVerts));
	}
	else
	{
		Resources::Mesh::Vertex* vertices = (Resources::Mesh::Vertex*)((char*)m_meshH + sizeof(MeshHeader));
		newMesh->SetVertices(vertices, this->m_Device, m_meshH->numVerts, true);
		indices = (unsigned int*)((char*)vertices + (sizeof(Resources::Mesh::Vertex)* m_meshH->numVerts));
	}

	if (!newMesh->SetIndices(indices, m_meshH->indexLength, this->m_Device, true))
		res = Resources::Status::ST_BUFFER_ERROR;
	
	/*we've already loaded one or more meshes into the scene*/
	//if (m_models.size() != 0)
	//{
	//	for (int i = 0; i < m_models.size(); ++i)
	//	{
	//		if (m_models.at(i)->GetMesh()->GetId() == res_Data->m_id)
	//		{
	//			newMesh->Destroy();
	//			delete newMesh;
	//			return;
	//		}
	//	}
	//	Resources::Model *m_new_model = new Resources::Model();

	//	m_new_model->SetMesh(newMesh);
	//	m_models.push_back(m_new_model);
	//}
	///*this is the first mesh loaded*/
	//else
	//{
	//	Resources::Model *m_new_model = new Resources::Model();

	//	m_new_model->SetMesh(newMesh);
	//	m_models.push_back(m_new_model);
	//}
	for (int i = 0; i < m_models.size(); ++i)
	{
		if (m_models.at(i)->GetRawModelData()->meshId == newMesh->GetId())
		{
			m_models.at(i)->SetMesh(newMesh);
		}
	}
	/*add to the ui here*/

	//Resources::Model *importedModel = new Resources::Model;
}

void FileImporter::handleMat(char * m_bbf_object)
{
	Resources::Status stat;
	Resources::Resource::RawResourceData * res_Data = (Resources::Resource::RawResourceData*)m_bbf_object;

	MaterialHeader* m_MatH = (MaterialHeader*)(m_bbf_object + sizeof(MainHeader));

	Resources::Material* newMaterial = new Resources::Material(*res_Data);
	//m_MatH->m_EmissiveValue;

	//Resources::Mesh::VertexAnim* vertices = (Resources::Mesh::VertexAnim)((char)m_meshH + sizeof(MeshHeader));
	//newMesh->SetVertices(vertices, nullptr, m_meshH->numVerts, true);
	//indices = (unsigned int)((char)vertices + (sizeof(Resources::Mesh::VertexAnim)* m_meshH->numVerts));
	newMaterial->SetMetallic(m_MatH->m_Metallic);
	newMaterial->SetRoughness(m_MatH->m_Roughness);
	newMaterial->SetEmissive(m_MatH->m_EmissiveValue);

	for (int i = 0; i < m_models.size(); ++i)
	{
		if (m_models.at(i)->GetRawModelData()->materialId == newMaterial->GetId())
		{
			m_models.at(i)->SetMaterial(newMaterial);
		}
	}
}

void FileImporter::handleModel(char * m_bbf_object)
{
	/*
	Resources::Status res;
	Resources::Resource::RawResourceData *res_Data = (Resources::Resource::RawResourceData*)m_bbf_object;

	MeshHeader *m_meshH = (MeshHeader*)(m_bbf_object + sizeof(MainHeader));

	Resources::Mesh *newMesh = new Resources::Mesh(*res_Data);


	unsigned int * indices;
	if (m_meshH->skeleton)
	{
		Resources::Mesh::VertexAnim* vertices = (Resources::Mesh::VertexAnim*)((char*)m_meshH + sizeof(MeshHeader));
		newMesh->SetVertices(vertices, nullptr, m_meshH->numVerts, true);
		indices = (unsigned int*)((char*)vertices + (sizeof(Resources::Mesh::VertexAnim)* m_meshH->numVerts));
	}
	else
	{
		Resources::Mesh::Vertex* vertices = (Resources::Mesh::Vertex*)((char*)m_meshH + sizeof(MeshHeader));
		newMesh->SetVertices(vertices, nullptr, m_meshH->numVerts, true);
		indices = (unsigned int*)((char*)vertices + (sizeof(Resources::Mesh::Vertex)* m_meshH->numVerts));
	}

	if (!newMesh->SetIndices(indices, m_meshH->indexLength, nullptr, true))
		res = Resources::Status::ST_BUFFER_ERROR; */
	Resources::Status res;
	Resources::Resource::RawResourceData *res_Data = (Resources::Resource::RawResourceData*)m_bbf_object;

	Resources::Model::RawModelData *raw_model_Data = (Resources::Model::RawModelData*)(m_bbf_object + sizeof(MainHeader));

	Resources::Model *newModel = new Resources::Model();

	newModel->Create(res_Data, raw_model_Data, true);

	m_models.push_back(newModel);
}

void FileImporter::AddListItem(ListItem category, std::string name)
{
	QTreeWidgetItem *itm = new QTreeWidgetItem();
	itm->setText(0, name.substr(0, name.rfind(".")).c_str());
	this->m_itemList->topLevelItem((int)category)->addChild(itm);

}
