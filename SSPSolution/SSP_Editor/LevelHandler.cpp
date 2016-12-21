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
	LevelData::MainLevelHeader header = this->GetMainHeader();
	char* data = (char*)&header;

	file.write(data, sizeof(LevelData::MainLevelHeader));

	file.close();

	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::ImportLevelFile()
{
	//Let the user pick a path and file name.
	std::string path = GetFilePathAndName(Operation::LOAD);

	std::fstream file;
	file.open(path, std::fstream::in | std::fstream::binary);

	//char* data = (char*)&this->GetMainHeader();
	LevelData::MainLevelHeader header;
	file.read((char*)&header, sizeof(LevelData::MainLevelHeader));

	file.close();

	 return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::NewLevel()
{
	
	this->m_currentLevel.Destroy();
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
			QFileInfo info(dlg.selectedFiles().at(0));
			std::string hej = info.baseName().toStdString();
			m_currentLevel.SetName(info.baseName().toStdString()); //Set the new name to the level
			
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

LevelData::MainLevelHeader LevelHandler::GetMainHeader()
{
	LevelData::MainLevelHeader header;
	
	header.resAmount	= m_currentLevel.GetUniqueModels()->size();
	header.entityAmount = m_currentLevel.GetNumEntities();
	header.lightAmount  = m_currentLevel.GetNumLights();
	return header;
}
