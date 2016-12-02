#include "FileImporter.h"



FileImporter::FileImporter()
{
	m_fileLoader = Resources::FileLoader::GetInstance();
}


FileImporter::~FileImporter()
{
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
	char *pek;
	size_t length;
	for (int i = 0; i < m_filepaths.size(); ++i)
	{
		m_fileLoader->LoadFile(m_filepaths.at(i), pek, &length);
	}
}
