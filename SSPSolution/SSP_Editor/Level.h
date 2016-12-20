#ifndef SSPEDITOR_LEVEL_LEVEL_H
#define SSPEDITOR_LEVEL_LEVEL_H
#include <unordered_map>
#include "Header.h"
#include "../../ResourceLib/Enumerations.h"

/*
	Author: Martin Clementson
	
	This Class contains all the data of a level. This class can be exported to a binary file and imported from a file.
	Only one level is active at a time. If one is imported while another is loaded, the loaded one is saved first.
*/
class Level
{
private:
	std::unordered_map<unsigned int, std::vector<Container>> m_ModelMap;
	std::unordered_map<unsigned int, std::vector<Container>> m_LightMap;
public:
	Level();
	~Level();
	

	std::unordered_map<unsigned int, std::vector<Container>> * GetModels();
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

};

#endif

