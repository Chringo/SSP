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

	struct SpawnHeader {
		float position[3];
		float rotation[3];
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
		L_FILE_NOT_FOUND,
		L_FILE_SAVE_CANCELLED
	};

	struct Level {
		SpawnHeader  spawns[2];
		unsigned int numResources;
		ResourceHeader* resources;

		unsigned int numEntities;
		EntityHeader* entities;

		unsigned int numLights;
		LightHeader* lights;

	};
}