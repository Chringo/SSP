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
		bool hasAi = false	   ;
	};

	struct SpawnHeader {
		float position[3];
		float rotation[3];
	};
	
	struct SceneLightHeader 
	{
		unsigned int numPointLights		  = 0;
		unsigned int numShadowCasters	  = 0;
		unsigned int numSpotLights		  = 0;
		unsigned int numDirectionalLights = 0;

		float ambientColor[3]  = { 1.0f,1.0f,1.0f };
		float ambientIntensity = 0.2f; 
	};
	//different lights will be added later

	struct ListenerHeader
	{
		int numConnections;
		int Event[20];
		unsigned int SenderID[20];
	};

	struct AiHeader : EntityHeader {
		int time = 0;
		float speed = 0;
		int pattern = 0;
		int nrOfWaypoints = 0;
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
		float rotateTime;
		ListenerHeader Listener;
	};

	struct LeverHeader : EntityHeader
	{
		float interactionDistance;
		ListenerHeader Listener;
	};

	struct WheelHeader : EntityHeader
	{
		float interactionDistance;
		float resetDelay;
		float resetTime;
		float min;
		float max;
		float time;
		ListenerHeader Listener;
	};

	enum LevelStatus {
		L_ERROR_CREATING_FILE,
		L_OK,
		L_FILE_NOT_FOUND,
		L_FILE_SAVE_CANCELLED
	};

	struct PointLightHeader {
		float position[3]		= { 0.0f,0.0f,0.0f };
		float color[3]			= { 1.0f,1.0f,1.0f };
		float radius			= 10.0f;
		float intensity			=  1.0f;
		float falloff_constant  =  1.0f;
		float falloff_linear    =  1.0f;
		float falloff_quadratic =  1.0f;
	};

	struct Level {
		SpawnHeader  spawns[2];
		unsigned int numResources;
		ResourceHeader* resources;

		unsigned int numEntities;
		EntityHeader* entities;

		unsigned int numAI;
		AiHeader* aiComponents;
		
		unsigned int numCheckpoints;
		CheckpointHeader* checkpoints;

		unsigned int numButton;
		ButtonHeader * buttons;

		unsigned int numDoor;
		DoorHeader * doors;

		unsigned int numLever;
		LeverHeader * levers;

		unsigned int numWheel;
		WheelHeader * wheels;
		
		float  ambientColor[3];
		float  ambientIntensity;

		unsigned int numPointLights;
		PointLightHeader* pointLights;

		
	};
}