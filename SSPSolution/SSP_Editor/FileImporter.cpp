#include "FileImporter.h"



FileImporter::FileImporter()
{
}


FileImporter::~FileImporter()
{
}

void FileImporter::ImportFromServer()
{
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir("//DESKTOP-BOKNO6D/server/Assets/bbf files/Meshes")) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			//printf("%s\n", ent->d_name);
			m_filepaths.push_back(ent->d_name);
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		perror("");
	}
	if ((dir = opendir("//DESKTOP-BOKNO6D/server/Assets/bbf files/Textures")) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			//printf("%s\n", ent->d_name);
			m_filepaths.push_back(ent->d_name);
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		perror("");
	}
	printf("tset");
}
