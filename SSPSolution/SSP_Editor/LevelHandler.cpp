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
	delete[] resData;															 //Release the data from memory

	//Spawn Points
	LevelData::SpawnHeader spawns[2];
	GetSpawnData((char*)spawns);
	file.write((char*)spawns, sizeof(LevelData::SpawnHeader) * 2);

	//Model Entities
	size_t modelSize = sizeof(LevelData::EntityHeader) * header.entityAmount;
	char* modelData  = new char[modelSize];					//Allocate for modelEntity data
	GetEntityData(modelData);								//Get modelEntity data	
	file.write(modelData, modelSize);						//Write all modelEntities	
	delete[] modelData;										//Release the data from memory
    
    //AI Entities
	if (header.AiComponentAmount > 0)
	{
		size_t aiSize = sizeof(LevelData::AiHeader) * header.AiComponentAmount;
		char* aiData = new char[aiSize];					    //Allocate for ai data
		GetAiData(aiData);								        //Get ai data	
		file.write(aiData, aiSize);							    //Write all aiComponents					
		delete[] aiData;
	}

	if (header.checkpointAmount > 0)
	{
		size_t checkpointSize = sizeof(LevelData::CheckpointHeader) * header.checkpointAmount;
		char* checkpointData  = new char[checkpointSize];
		this->GetCheckpointData(checkpointData);
		file.write(checkpointData, checkpointSize);
		delete[] checkpointData;
	}

	if (header.buttonAmount > 0)
	{
		size_t buttonSize = sizeof(LevelData::ButtonHeader) * header.buttonAmount;
		char* buttonData = new char[buttonSize];
		this->GetButtonData(buttonData);
		file.write(buttonData, buttonSize);
		delete[] buttonData;
	}
	if (header.doorAmount > 0)
	{
		size_t doorSize = sizeof(LevelData::DoorHeader) * header.doorAmount;
		char* doorData = new char[doorSize];
		this->GetDoorData(doorData);
		file.write(doorData, doorSize);
		delete[] doorData;
	}
	if (header.leverAmount > 0)
	{
		size_t leverSize = sizeof(LevelData::LeverHeader) * header.leverAmount;
		char* leverData = new char[leverSize];
		this->GetLeverData(leverData);
		file.write(leverData, leverSize);
		delete[] leverData;
	}
	if (header.wheelAmount > 0)
	{
		size_t wheelSize = sizeof(LevelData::WheelHeader) * header.wheelAmount;
		char* wheelData = new char[wheelSize];
		this->GetWheelData(wheelData);
		file.write(wheelData, wheelSize);
		delete[] wheelData;
	}

	
	LevelData::SceneLightHeader sceneLights = GetSceneLightHeader(); //Get the lights header for the file 
	char* sceneLightdata = (char*)&sceneLights;
	file.write(sceneLightdata, sizeof(LevelData::SceneLightHeader)); //Write the scene light header

	if (sceneLights.numPointLights > 0)
	{
		size_t pointLightSize = sizeof(LevelData::PointLightHeader) * sceneLights.numPointLights;
		char* pointData		  = new char[pointLightSize];
		this->GetPointLightData(pointData);
		file.write(pointData, pointLightSize);
		delete[] pointData;
	}



	file.close();
	//Cleanup

	

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
	if (header.AiComponentAmount > 0)
	{
		size_t aiSize = sizeof(LevelData::AiHeader) * header.AiComponentAmount;
		char* aiData = new char[aiSize];					    //Allocate for ai data
		file.read(aiData, aiSize);							//read all aiComponents	
		
		//TODO: LOAD INTO LEVEL
		LoadAiComponents((LevelData::AiHeader*)aiData, header.AiComponentAmount);
		
		delete aiData;
	}
	if (header.checkpointAmount > 0)
	{
		size_t checkpointSize = sizeof(LevelData::CheckpointHeader) * header.checkpointAmount;
		char* checkpointData = new char[checkpointSize];
		file.read(checkpointData, checkpointSize);

		LoadCheckpointComponents((LevelData::CheckpointHeader*)checkpointData, header.checkpointAmount);
		delete checkpointData;
	}
	if (header.buttonAmount > 0)
	{
		size_t buttonSize = sizeof(LevelData::ButtonHeader) * header.buttonAmount;
		char* buttonData = new char[buttonSize];
		file.read(buttonData, buttonSize);

		LoadTriggerComponents((LevelData::ButtonHeader*)buttonData, header.buttonAmount);
		delete buttonData;
	}
	if (header.doorAmount > 0)
	{
		size_t doorSize = sizeof(LevelData::DoorHeader) * header.doorAmount;
		char* doorData = new char[doorSize];
		file.read(doorData, doorSize);

		LoadTriggerComponents((LevelData::DoorHeader*)doorData, header.doorAmount);
		delete doorData;
	}

	if (header.leverAmount > 0)
	{
		size_t leverSize = sizeof(LevelData::LeverHeader) * header.leverAmount;
		char* leverData = new char[leverSize];
		file.read(leverData, leverSize);

		LoadTriggerComponents((LevelData::LeverHeader*)leverData, header.leverAmount);
		delete leverData;
	}
	if (header.wheelAmount > 0)
	{
		size_t wheelSize = sizeof(LevelData::WheelHeader) * header.wheelAmount;
		char* wheelData = new char[wheelSize];
		file.read(wheelData, wheelSize);

		LoadTriggerComponents((LevelData::WheelHeader*)wheelData, header.wheelAmount);
		delete wheelData;
	}
	if (file.eof() == false) { // if we havent reached the end of file here, then we're using the new levels with lights

	//Light header
		
		LevelData::SceneLightHeader lightHeader;
		file.read((char*)&lightHeader, sizeof(LevelData::SceneLightHeader));
		LIGHTING::LightHandler::GetInstance()->SetAmbientLight(
			lightHeader.ambientColor[0],
			lightHeader.ambientColor[1],
			lightHeader.ambientColor[2],
			lightHeader.ambientIntensity
			);
		//Point lights
		if (lightHeader.numPointLights > 0) {

		size_t pointlightSize = sizeof(LevelData::PointLightHeader) * lightHeader.numPointLights;	  //memsize
		char* pointData = new char[pointlightSize];
		file.read(pointData, pointlightSize);
		LoadPointLightComponents((LevelData::PointLightHeader*)pointData, lightHeader.numPointLights);
		delete pointData;
		}


	}
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
	
	header.entityAmount      = m_currentLevel.GetNumEntities();
	header.lightAmount       = m_currentLevel.GetNumLights();
	header.resAmount	     = (unsigned int) m_currentLevel.GetUniqueModels()->size();
	header.AiComponentAmount = (unsigned int) m_currentLevel.GetAiHandler()->GetAllPathComponents()->size();
	header.checkpointAmount  = (unsigned int) m_currentLevel.GetCheckpoints()->size();
	header.buttonAmount		 = (unsigned int) m_currentLevel.GetPuzzleElements(BUTTON)->size();
	header.doorAmount		 = (unsigned int) m_currentLevel.GetPuzzleElements(DOOR)->size();
	header.leverAmount		 = (unsigned int) m_currentLevel.GetPuzzleElements(LEVER)->size();
	header.wheelAmount		 = (unsigned int) m_currentLevel.GetPuzzleElements(WHEEL)->size();

	return header;
}

LevelData::SceneLightHeader LevelHandler::GetSceneLightHeader()
{
	LevelData::SceneLightHeader data;

	data.numPointLights = LightController::GetInstance()->GetPointLightData()->size();
	
	//TODO: Add ambientColor and ambient Intesity
	return data;
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
			//if (entityContainer->at(i).aiComponent != nullptr)
			//{
			//	entity.isStatic = false;
			//	entity.hasAi = true;
			//}
			//else
				entity.hasAi = false;
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
	
	for each (AiContainer* ai in *m_currentLevel.GetAiHandler()->GetAllPathComponents())
	{
		LevelData::AiHeader * ah = ai->GetData();
		memcpy(dataPtr + offset, (char*)ah, sizeof(LevelData::AiHeader));
		offset += sizeof(LevelData::AiHeader);
	}

	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::GetCheckpointData(char * dataPtr)
{
	unsigned int offset = 0;
	LevelData::CheckpointHeader * cph;
	for (int i = 0; i < m_currentLevel.GetCheckpoints()->size(); i++)
	{
		cph = m_currentLevel.GetCheckpoints()->at(i)->GetDataPtr();
		memcpy(dataPtr + offset, (char*)cph, sizeof(LevelData::CheckpointHeader));
		offset += sizeof(LevelData::CheckpointHeader);
	}

	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::GetButtonData(char * dataPtr)
{
	unsigned int offset = 0;
	for each (Button* button in *this->m_currentLevel.GetPuzzleElements(BUTTON))
	{
		LevelData::ButtonHeader* bh = button->GetData();
		memcpy(dataPtr + offset, (char*)bh, sizeof(LevelData::ButtonHeader));
		offset += sizeof(LevelData::ButtonHeader);
	}

	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::GetDoorData(char * dataPtr)
{
	unsigned int offset = 0;
	for each (Door* door in *this->m_currentLevel.GetPuzzleElements(DOOR))
	{
		LevelData::DoorHeader* bh = door->GetData();
		memcpy(dataPtr + offset, (char*)bh, sizeof(LevelData::DoorHeader));
		offset += sizeof(LevelData::DoorHeader);
	}

	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::GetLeverData(char * dataPtr)
{
	unsigned int offset = 0;
	for each (Lever* lever in *this->m_currentLevel.GetPuzzleElements(LEVER))
	{
		LevelData::LeverHeader* lh = lever->GetData();
		memcpy(dataPtr + offset, (char*)lh, sizeof(LevelData::LeverHeader));
		offset += sizeof(LevelData::LeverHeader);
	}

	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::GetWheelData(char * dataPtr)
{
	unsigned int offset = 0;
	for each (Wheel* wheel in *this->m_currentLevel.GetPuzzleElements(WHEEL))
	{
		LevelData::WheelHeader* wh = wheel->GetData();
		memcpy(dataPtr + offset, (char*)wh, sizeof(LevelData::WheelHeader));
		offset += sizeof(LevelData::WheelHeader);
	}

	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::GetPointLightData(char * dataPtr)
{
	unsigned int offset = 0;
	std::vector<LIGHTING::Point>* lights = LightController::GetInstance()->GetPointLightData();

	for each(LIGHTING::Point light in *lights) {
		
		LevelData::PointLightHeader currentLight;
		currentLight.falloff_quadratic = light.falloff.quadratic;
		currentLight.falloff_constant  = light.falloff.constant;
		currentLight.falloff_linear    = light.falloff.linear;
		currentLight.position[0]	   = light.position.m128_f32[0];
		currentLight.position[1]	   = light.position.m128_f32[1];
		currentLight.position[2]	   = light.position.m128_f32[2];
		currentLight.intensity		   = light.intensity;
		currentLight.color[0]		   = light.color.r;
		currentLight.color[1]		   = light.color.g;
		currentLight.color[2]		   = light.color.b;
		currentLight.radius			   = light.radius;
		memcpy(dataPtr + offset, (char*)&currentLight, sizeof(LevelData::PointLightHeader));
		offset += sizeof(LevelData::PointLightHeader);
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

LevelData::LevelStatus LevelHandler::LoadAiComponents(LevelData::AiHeader * dataPtr, size_t numComponents)
{
	for (size_t i = 0; i < numComponents; i++)
	{
		AiContainer* newComponent = new AiContainer(&dataPtr[i]);
		m_currentLevel.AddPuzzleElement(AI, newComponent);
			
		newComponent->component.modelPtr = DataHandler::GetInstance()->GetModel(newComponent->component.modelID);
	}

	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::LoadCheckpointComponents(LevelData::CheckpointHeader * dataPtr, size_t numComponents)
{
	for (size_t i = 0; i < numComponents; i++)
	{
		CheckpointContainer * checkpoint = new CheckpointContainer(dataPtr[i]);

		if (checkpoint->position.m128_f32[0] < -9999999)
			checkpoint->position = { 0.0,0.0,0.0 };

		checkpoint->Update();
		m_currentLevel.GetCheckpoints()->push_back(checkpoint);
	}

	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::LoadPointLightComponents(LevelData::PointLightHeader * dataPtr, size_t numComponents)
{


	for (size_t i = 0; i < numComponents; i++)
	{
		LIGHTING::Point point;
		memcpy(&point.color, dataPtr[i].color, sizeof(float) * 3);
		memcpy(&point.position.m128_f32, dataPtr[i].position, sizeof(float) * 3);

		point.falloff.quadratic = dataPtr[i].falloff_quadratic;
		point.falloff.constant  = dataPtr[i].falloff_constant;
		point.falloff.linear	= dataPtr[i].falloff_linear;
		point.intensity			= dataPtr[i].intensity;
		point.radius			= dataPtr[i].radius;

		LightController::GetInstance()->AddLight(&point);
	}
	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::LoadTriggerComponents(LevelData::ButtonHeader * dataPtr, size_t numComponents)
{
	for (size_t i = 0; i < numComponents; i++)
	{
		Button * button = new Button(&dataPtr[i]);
		m_currentLevel.AddPuzzleElement(BUTTON, button);

		button->component.modelPtr = DataHandler::GetInstance()->GetModel(button->component.modelID);
	}

	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::LoadTriggerComponents(LevelData::DoorHeader * dataPtr, size_t numComponents)
{
	for (size_t i = 0; i < numComponents; i++)
	{
		Door * door = new Door(&dataPtr[i]);
		m_currentLevel.AddPuzzleElement(DOOR, door);

		door->component.modelPtr = DataHandler::GetInstance()->GetModel(door->component.modelID);
	}

	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::LoadTriggerComponents(LevelData::WheelHeader * dataPtr, size_t numComponents)
{
	for (size_t i = 0; i < numComponents; i++)
	{
		Wheel * wheel = new Wheel(&dataPtr[i]);
		m_currentLevel.AddPuzzleElement(WHEEL, wheel);

		wheel->component.modelPtr = DataHandler::GetInstance()->GetModel(wheel->component.modelID);
	}

	return LevelData::LevelStatus::L_OK;
}

LevelData::LevelStatus LevelHandler::LoadTriggerComponents(LevelData::LeverHeader * dataPtr, size_t numComponents)
{
	for (size_t i = 0; i < numComponents; i++)
	{
		Lever * lever = new Lever(&dataPtr[i]);
		m_currentLevel.AddPuzzleElement(LEVER, lever);

		lever->component.modelPtr = DataHandler::GetInstance()->GetModel(lever->component.modelID);
	}

	return LevelData::LevelStatus::L_OK;
}
