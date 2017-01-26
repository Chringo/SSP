#pragma once

namespace LevelData {


	//This is used to convert levels from earlier versions of the editor

	struct OLDMainLevelHeader
	{
		unsigned int resAmount;
		unsigned int entityAmount;
		unsigned int lightAmount;
		unsigned int AiComponentAmount;
		unsigned int checkpointAmount;
	};
	struct MainLevelHeader
	{
		unsigned int resAmount;
		unsigned int entityAmount;
		unsigned int lightAmount;
		unsigned int AiComponentAmount;
		unsigned int checkpointAmount;
		
		unsigned int buttonAmount;
		unsigned int doorAmount;
		unsigned int leverAmount;
		unsigned int wheelAmount;
	};
	
	struct ResourceHeader {
		unsigned int id;
		int resourceType;
	};

	struct OldEntityHeader
	{
		unsigned int modelID;
		unsigned int EntityID;
		float position[3];
		float rotation[3];
		bool isStatic;
	};
	
	struct EntityHeader
	{
		unsigned int modelID   ;
		unsigned int EntityID  ;
		float position[3]	   ;
		float rotation[3]	   ;
		bool isStatic		   ;
		bool hasAi = false;
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

	struct ListenerHeader
	{
		int numConnections;
		int Event[20];
		int SenderID[20];
	};

	struct AiHeader{
		unsigned int entityID = -1;
		int time			  = 0;
		float speed			  = 0;
		int pattern			  = 0;
		int nrOfWaypoints     = 0;
		float wayPoints[8][3];
		ListenerHeader Listener;
	};

	struct CheckpointHeader
	{
		unsigned int entityID; 
		unsigned int checkpointNumber = 0;
		float position[3];
		float ext[3];
		float ort[16];
	};
	
	struct ButtonHeader : EntityHeader
	{
		float interactionDistance;
		float resetTime;
		ListenerHeader Listener;
	};

	struct DoorHeader : EntityHeader
	{
		float RotateTimer;
	};

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

		unsigned int numAI;
		AiHeader* aiComponents;
		
		unsigned int numCheckpoints;
		CheckpointHeader* checkpoints;
	};
}