#include "FileImporter.h"



FileImporter::FileImporter()
{
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
	if ((dir = opendir("//DESKTOP-BOKNO6D/server/Assets/bbf files/Meshes")) != NULL) 
	{
		/* append all the mesh names from the directory */
		while ((ent = readdir(dir)) != NULL) 
		{
			if (*ent->d_name != '.')
			{
				std::string pathName = "//DESKTOP-BOKNO6D/server/Assets/bbf files/Meshes";
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
		m_fileLoader->LoadFile(m_filepaths.at(i), m_bbf_object, &length);

		Resources::ResourceType loadedObject = (Resources::ResourceType)(*m_bbf_object + sizeof(unsigned int));

		switch (loadedObject)
		{
		case Resources::ResourceType::RES_MESH:
			handleMesh(m_bbf_object);
			break;
		case Resources::ResourceType::RES_ANIMATION:
			break;
		case Resources::ResourceType::RES_MODEL:
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

void FileImporter::handleMesh(char * m_bbf_object)
{
	/*create model type here and then when reading */

	/*add to the ui here*/

	//Resources::Model *importedModel = new Resources::Model;
}

void FileImporter::handleMat(char * m_bbf_object)
{
}
