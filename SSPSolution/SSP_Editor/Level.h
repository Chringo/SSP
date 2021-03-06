#ifndef SSPEDITOR_LEVEL_LEVEL_H
#define SSPEDITOR_LEVEL_LEVEL_H
#include <unordered_map>
#include "Header.h"
#include "../../ResourceLib/Enumerations.h"
#include "DataHandler.h"
#include "AiHandler.h"
#include "GlobalIDHandler.h"
#include "Checkpointhandler.h"
#include "LightController.h"
//#include "UiControlHandler.h"
#define PLAYER1 2215164276 
#define PLAYER2 3255160373
/*
	Author: Martin Clementson
	
	This Class contains all the data of a level. This class can be exported to a binary file and imported from a file.
	Only one level is active at a time. If one is imported while another is loaded, the loaded one is saved first.

	Some functions have been transfered from "EditorCommunicator" where Johan is the author
*/
class Level
{
private:
	Container m_SpawnPoints[2];
	AiHandler m_LevelAi; //Handler for the AI in the level
	CheckpointHandler m_checkpointHandler;
	std::string levelName = "untitled_level";
	std::vector<unsigned int> m_uniqueModels;							 // Every unique model used by the level
	std::unordered_map<unsigned int, std::vector<Container>> m_ModelMap; // Every instance of modelEntities in the level
	std::vector<std::vector<Container*>> m_puzzleElements;				 //A 2d array of MODEL,BUTTON,LEVER,WHEEL,DOOR,MAGNET,PRESSUREPLATE, Use enum to access									 //all ze lightz
	GraphicsHandler* m_graphicsHandler = nullptr;
public:
	Level();
	~Level();
	
	std::vector<unsigned int>* GetUniqueModels() { return &this->m_uniqueModels; };
	std::unordered_map<unsigned int, std::vector<Container>> * GetModelEntities();
	std::vector<Light*> * GetLights();
	std::vector<CheckpointContainer*>* GetCheckpoints();
	Container* GetInstanceEntity(unsigned int entityID);
	Resources::Status GetModelEntity(unsigned int modelID, unsigned int instanceID, Container& container);
	Resources::Status AddModelEntity(unsigned int modelID,  DirectX::XMVECTOR position, DirectX::XMVECTOR rotation);
	Resources::Status AddModelEntityFromLevelFile(unsigned int modelID, unsigned int instanceID,DirectX::XMVECTOR position, DirectX::XMVECTOR rotation);
	Resources::Status AddCheckpointEntity();
	Resources::Status AddPuzzleElement(ContainerType type, void * element);
	Resources::Status UpdateModel(unsigned int modelID, unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation);
	Resources::Status UpdateSpawnPoint(unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation);
	Resources::Status UpdateCheckpoint(unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMVECTOR scale);
	Resources::Status AddPointLight();
	Resources::Status RemoveModel(unsigned int modelID, unsigned int instanceID);
	Resources::Status DuplicateEntity(  Container*& source, Container*& destination);
	void generateCubeMap(GraphicsHandler* gh, DirectX::XMVECTOR campos);
/*
	Resources::Status GetLightEntity(unsigned int instanceID, Container& container);
	Resources::Status AddLightEntity(unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation);
	Resources::Status UpdateLight( unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation);
	Resources::Status RemoveLight( unsigned int instanceID);
*/
	bool isEmpty();
	unsigned int GetNumEntities();
	unsigned int GetNumLights();
	unsigned int GetNumPuzzleElements();
	Container* GetSpawnPoint(int index);
	AiHandler* GetAiHandler() { return &m_LevelAi; };
	CheckpointHandler* GetCheckpointHandler() { return &this->m_checkpointHandler; };
	
	const std::string* GetName() { return &levelName; };
	void SetName(std::string& newName) { this->levelName = newName; };
	void Destroy(); //Clears the whole level, This is used when a new scene is loaded
	void SetSpawnPoint(LevelData::SpawnHeader data, int index);

	const std::vector<Container*>* GetPuzzleElements(ContainerType type);
	AiContainer* ConvertToAI(Container*& object);
	Container*   ConvertToContainer(Container*& object); //polymorphism 
	Button*      ConvertToButton(Container*& object);
	Wheel*		 ConvertToWheel (Container*& object);
	Lever*		 ConvertToLever (Container*& object);
	Door*        ConvertToDoor  (Container*& object);
	

	
};

#endif

