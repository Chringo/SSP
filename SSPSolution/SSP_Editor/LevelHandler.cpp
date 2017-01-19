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
		return LevelData::LevelStatus::L_FILE_SAVE_CANCELLED;
	}

	std::fstream file;
	file.open(path, std::fstream::out | std::fstream::binary);
	LevelData::MainLevelHeader header = this->GetMainHeader();				//Get the main header for the file 
	char* data = (char*)&header;

	file.write(data, sizeof(LevelData::MainLevelHeader));					 //Write the main header first

	//Resource data
	size_t resSize = sizeof(LevelData::ResourceHeader)* header.resAmount;    //size of resource data
	char* resData  = new char[resSize];										 //Allocate for resource data
	GetResourceData(resData);												 //Get resource data
	file.write(resData, resSize);											 //Write resource data to file

	//Spawn Points
	LevelData::SpawnHeader spawns[2];
	GetSpawnData((char*)spawns);
	file.write((char*)spawns, sizeof(LevelData::SpawnHeader) * 2);

	//Model Entities
	size_t modelSize = sizeof(LevelData::EntityHeader) * header.entityAmount;
	char* modelData  = new char[modelSize];					//Allocate for modelEntity data
	GetEntityData(modelData);								//Get modelEntity data	
	file.write(modelData, modelSize);						//Write all modelEntities														
    
    //AI Entities
	if (header.AiComponentAmount > 0)
	{
		size_t aiSize = sizeof(LevelData::AiHeader) * header.AiComponentAmount;
		char* aiData = new char[aiSize];					    //Allocate for ai data
		GetAiData(aiData);								        //Get ai data	
		file.write(aiData, aiSize);						//Write all aiComponents					
		delete aiData;
	}

	file.close();
	//Cleanup
	delete resData;
	delete modelData;

	QFileInfo info(QString::fromStdString(path));
	m_currentLevel.SetName(info.baseName().toStdString()); //Set the new name to the level
	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::ImportLevelFile()
{
	
	//Let the user pick a path and file name.
	std::string path = GetFilePathAndName(Operation::LOAD);
	if (path == "")
	{
		return LevelData::LevelStatus::L_FILE_SAVE_CANCELLED;
	}
	NewLevel();		//Empty the current level object
																		 
	std::fstream file;													
	file.open(path, std::fstream::in | std::fstream::binary);			


	LevelData::MainLevelHeader header;
	file.read((char*)&header, sizeof(LevelData::MainLevelHeader));				 //Read file header

	//Resource data
	size_t resSize = sizeof(LevelData::ResourceHeader)* header.resAmount;		  //size of resource data
	file.seekg(resSize, std::ios_base::cur);									  //Skip the resource data (only used by the game engine)
	
	//Spawn Points
	LevelData::SpawnHeader spawns[2];
	file.read((char*)spawns, sizeof(LevelData::SpawnHeader) * 2);
	m_currentLevel.SetSpawnPoint(spawns[0], 0);
	m_currentLevel.SetSpawnPoint(spawns[1], 1);

	//Model Entities
	size_t modelSize = sizeof(LevelData::EntityHeader) * header.entityAmount;	  //memsize
	char* modelData = new char[modelSize];										  //allocate for the entities
	file.read(modelData, modelSize);											  //Bulk read all the entity data
	this->LoadEntities((LevelData::EntityHeader*)modelData, header.entityAmount); //Load them into the level object

	//AI Entities


	file.close();
	delete modelData; //Cleanup
	QFileInfo info(QString::fromStdString(path));
	m_currentLevel.SetName(info.baseName().toStdString()); //Set the  name to the level

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
			return dlg.selectedFiles().at(0).toStdString(); //Return file path as string
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
	
	header.resAmount	     = m_currentLevel.GetUniqueModels()->size();
	header.entityAmount      = m_currentLevel.GetNumEntities();
	header.lightAmount       = m_currentLevel.GetNumLights();
	header.AiComponentAmount = (unsigned int) m_currentLevel.GetAiHandler()->GetAllPathComponents()->size();
	return header;
}

LevelData::LevelStatus LevelHandler::GetEntityData(char * dataPtr)
{
	unsigned int offset = 0;
	std::unordered_map<unsigned int, std::vector<Container>> *  map = m_currentLevel.GetModelEntities();
	for (auto iterator = map->begin(); iterator != map->end(); ++iterator)
	{
		std::vector<Container> * entityContainer = &iterator->second;

		for (size_t i = 0; i < entityContainer->size(); i++)
		{
			LevelData::EntityHeader entity;

			entity.EntityID = entityContainer->at(i).internalID;
			entity.modelID  = entityContainer->at(i).component.modelID;
			
			entity.position[0] = entityContainer->at(i).position.m128_f32[0];
			entity.position[1] = entityContainer->at(i).position.m128_f32[1];	//Convert from Vector to float3
			entity.position[2] = entityContainer->at(i).position.m128_f32[2];
			

			entity.rotation[0] = entityContainer->at(i).rotation.m128_f32[0];
			entity.rotation[1] = entityContainer->at(i).rotation.m128_f32[1];	//Convert from Vector to float3
			entity.rotation[2] = entityContainer->at(i).rotation.m128_f32[2];

			entity.isStatic	   = entityContainer->at(i).isStatic;
			memcpy(dataPtr + offset, (char*)&entity, sizeof(LevelData::EntityHeader));
			offset += sizeof(LevelData::EntityHeader);

		}
	
	}
	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::GetResourceData(char * dataPtr)
{
	unsigned int offset = 0;
	for (size_t i = 0; i < this->m_currentLevel.GetUniqueModels()->size(); i++)
	{
		LevelData::ResourceHeader res;
		res.id = this->m_currentLevel.GetUniqueModels()->at(i);
		res.resourceType = Resources::ResourceType::RES_MODEL;
		memcpy(dataPtr + offset, (char*)&res, sizeof(LevelData::ResourceHeader));
		offset += sizeof(LevelData::ResourceHeader);
	}


	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::GetSpawnData(char * dataPtr)
{
	
	for (size_t i = 0; i < 2; i++)
	{
		Container* spawn = m_currentLevel.GetSpawnPoint(i);
		LevelData::SpawnHeader spawnheader;

		spawnheader.position[0] = spawn->position.m128_f32[0];
		spawnheader.position[1] = spawn->position.m128_f32[1];	//Convert from Vector to float3
		spawnheader.position[2] = spawn->position.m128_f32[2];

		spawnheader.rotation[0] = spawn->rotation.m128_f32[0];
		spawnheader.rotation[1] = spawn->rotation.m128_f32[1];	//Convert from Vector to float3
		spawnheader.rotation[2] = spawn->rotation.m128_f32[2];

		memcpy(dataPtr + sizeof(LevelData::SpawnHeader)* i, (char*)&spawnheader,sizeof(LevelData::SpawnHeader));


	}
	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::GetAiData(char * dataPtr)
{
	unsigned int offset = 0;
	std::vector<AIComponent*>* aiData = m_currentLevel.GetAiHandler()->GetAllPathComponents();
	for (size_t i = 0; i < aiData->size(); i++) // for each ai component in the level
	{
		LevelData::AiHeader ai;
		ai.entityID		 = aiData->at(i)->AC_entityID;
		ai.nrOfWaypoints = aiData->at(i)->AC_nrOfWaypoint;
		ai.pattern		 = aiData->at(i)->AC_pattern;
		ai.speed		 = aiData->at(i)->AC_speed;
		ai.time			 = aiData->at(i)->AC_time;
		memset(ai.wayPoints, 0, sizeof(float) * 24);

		for (size_t j = 0; j < ai.nrOfWaypoints; j++)
		{
				ai.wayPoints[j][0] = aiData->at(i)->AC_waypoints[j].m128_f32[0];
				ai.wayPoints[j][1] = aiData->at(i)->AC_waypoints[j].m128_f32[1];
				ai.wayPoints[j][2] = aiData->at(i)->AC_waypoints[j].m128_f32[2];
		}
	}


	for (size_t i = 0; i < this->m_currentLevel.GetUniqueModels()->size(); i++)
	{
		LevelData::ResourceHeader res;
		res.id = this->m_currentLevel.GetUniqueModels()->at(i);
		res.resourceType = Resources::ResourceType::RES_MODEL;
		memcpy(dataPtr + offset, (char*)&res, sizeof(LevelData::ResourceHeader));
		offset += sizeof(LevelData::ResourceHeader);
	}

	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::LoadEntities(LevelData::EntityHeader* dataPtr, size_t numEntities)
{
	GlobalIDHandler::GetInstance()->ResetIDs();
	for (size_t i = 0; i < numEntities; i++)
	{
		m_currentLevel.AddModelEntityFromLevelFile(dataPtr[i].modelID,
			dataPtr[i].EntityID,
			DirectX::XMVectorSet(dataPtr[i].position[0], dataPtr[i].position[1], dataPtr[i].position[2], 0.0),
			DirectX::XMVectorSet(dataPtr[i].rotation[0], dataPtr[i].rotation[1], dataPtr[i].rotation[2], 0.0));

	}

	return LevelData::LevelStatus::L_OK;
}
