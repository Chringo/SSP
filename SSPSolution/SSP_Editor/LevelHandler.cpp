#include "LevelHandler.h"



LevelHandler::LevelHandler()
{
}


LevelHandler::~LevelHandler()
{
}

LevelHandler * LevelHandler::GetInstance()
{
	static LevelHandler instance;
	return &instance;
}

LevelData::LevelStatus LevelHandler::ExportLevelFile()
{
	std::string path = GetFilePathAndName(Operation::SAVE);
	if (path == "")
	{
		return LevelData::LevelStatus::L_FILE_SAVE_CANCELED;
	}

	std::fstream file;
	file.open(path, std::fstream::out | std::fstream::binary);

	char hej = 'e';
	file.write(&hej, 1);
	file.close();
	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::ImportLevelFile()
{
	//Let the user pick a path and file name.
	std::string fileName = GetFilePathAndName(Operation::LOAD);


	 return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::NewLevel()
{
	
	
	return LevelData::LevelStatus::L_OK;
}



std::string LevelHandler::GetFilePathAndName(Operation flag)
{
	if (flag == Operation::SAVE)
	{
		QFileDialog dlg(NULL, "Save file");
		dlg.setAcceptMode(QFileDialog::AcceptSave);
		QString fullpath = filePath;
		fullpath.append(m_currentLevel.GetName()->c_str());
		fullpath.append(".level");
	
		dlg.setDirectory(fullpath);
		dlg.setNameFilter("Levels (*.level)");
		if (dlg.exec())
		{
			return dlg.selectedFiles().at(0).toStdString();
		}
		else
		{
			return "";
		}
	}

	else
	{
		QFileDialog dlg(NULL, "Load file");
		dlg.setAcceptMode(QFileDialog::AcceptOpen);
		//QFileDialog::ReadOnly
		
		dlg.setDirectory(filePath);
		dlg.setNameFilter("Levels (*.level)");
		if (dlg.exec())
		{
			return dlg.selectedFiles().at(0).toStdString();
		}
		else
		{
			return "";
		}

	}
	

}
