#pragma once

namespace LevelData {
	struct MainLevelHeader
	{
		unsigned int resAmount;
		unsigned int entityAmount;
		unsigned int lightAmount;
	};
	
	struct ResourceHeader {
		unsigned int id;
		int resourceType;
	};
	
	struct EntityHeader
	{
		unsigned int modelID   ;
		unsigned int EntityID  ;
		float position[3]	   ;
		float rotation[3]	   ;
		bool isStatic		   ;
	};
	
	struct LightHeader 
	{
		int lightType;
		float position[3];
		float rotation[3];
	};
	//different lights will be added later

	enum LevelStatus {

		L_ERROR_CREATING_FILE,
		L_OK,
		L_FILE_NOT_FOUND
	};
}