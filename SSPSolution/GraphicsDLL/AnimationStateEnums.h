#ifndef GRAPHICSDLL_ANIMATIONS_ANIMATIONSTATEENUMS_H
#define GRAPHICSDLL_ANIMATIONS_ANIMATIONSTATEENUMS_H

#ifdef GRAPHICSDLL_EXPORTS
#define GRAPHICSDLL_API __declspec(dllexport)
#else
#define GRAPHICSDLL_API __declspec(dllimport)
#endif

enum AnimationStates : int
{
	ANIMATION_ERROR = -1,
	PLAYER_IDLE = 0,
	PLAYER_WALK,
	PLAYER_RUN,  
	PLAYER_THROW, 
	PLAYER_JUMP,
	LEVEL_DEACTIVATED = 0,
	LEVEL_ACTIVATED,
	DOOR_OPENING = 0, 
	DOOR_CLOSING 
};

enum Blending
{
	NO_TRANSITION = 0,
	SMOOTH_TRANSITION = 1
	//FROZEN_TRANSITION	=	2
};

#endif