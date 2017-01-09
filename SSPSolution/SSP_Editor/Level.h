#ifndef SSPEDITOR_LEVEL_LEVEL_H
#define SSPEDITOR_LEVEL_LEVEL_H
#include <unordered_map>
#include "Header.h"
#include "../../ResourceLib/Enumerations.h"

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
	std::string levelName = "untitled_level";
	std::vector<unsigned int> m_uniqueModels;							 //Every unique model used by the level
	std::unordered_map<unsigned int, std::vector<Container>> m_ModelMap; //Every instance of modelEntities in the level
	std::unordered_map<unsigned int, std::vector<Container>> m_LightMap; // Every instance of lights in the level
public:
	Level();
	~Level();
	
	std::vector<unsigned int>* GetUniqueModels() { return &this->m_uniqueModels; };
	std::unordered_map<unsigned int, std::vector<Container>> * GetModelEntities();
	std::unordered_map<unsigned int, std::vector<Container>> * GetLights();

	Resources::Status GetModelEntity(unsigned int modelID, unsigned int instanceID, Container& container);
	Resources::Status AddModelEntity(unsigned int modelID, unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation);
	Resources::Status UpdateModel(unsigned int modelID, unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation);
	Resources::Status RemoveModel(unsigned int modelID, unsigned int instanceID);
	

/*
	Resources::Status GetLightEntity(unsigned int instanceID, Container& container);
	Resources::Status AddLightEntity(unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation);
	Resources::Status UpdateLight( unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation);
	Resources::Status RemoveLight( unsigned int instanceID);
*/
	bool isEmpty();
	unsigned int GetNumEntities();
	unsigned int GetNumLights();
	const std::string* GetName() { return &levelName; };
	void SetName(std::string& newName) { this->levelName = newName; };
	void Destroy(); //Clears the whole level, This is used when a new scene is loaded
};

#endif

