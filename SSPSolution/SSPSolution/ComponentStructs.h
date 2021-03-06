#ifndef SSPAPPLICATION_COMPONENT_COMPONENTSTRUCTS
#define SSPAPPLICATION_COMPONENT_COMPONENTSTRUCTS
#include <DirectXMath.h>
#include <vector>

enum EVENT
{
	FIELD_CONTAINS,				/*USED*/
	FIELD_ENTERED,				/*NOT	USED*/
	FIELD_EXITED,				/*NOT	USED*/
	FIELD_CONDITIONS_MET,		/*NOT	USED*/
	FIELD_ENABLED,				/*NOT	USED*/
	FIELD_DISABLED,				/*NOT	USED*/
	BUTTON_DEACTIVE,			/*USED*/
	BUTTON_ACTIVE,				/*USED*/
	BUTTON_RESET,				/*NOT	USED*/
	BUTTON_ENABLED,				/*NOT	USED*/
	BUTTON_DISABLED,			/*NOT	USED*/
	LEVER_DEACTIVE,				/*USED*/
	LEVER_ACTIVE,				/*USED*/
	LEVER_ENABLED,				/*NOT	USED*/
	LEVER_DISABLED,				/*NOT	USED*/
	DOOR_CLOSED,				/*USED*/
	DOOR_OPENED,				/*USED*/
	DOOR_REMOVED,				/*NOT	USED*/
	DOOR_ENABLED,				/*NOT	USED*/
	DOOR_DISABLED,				/*NOT	USED*/
	WHEEL_DECREASING,			/*USED*/
	WHEEL_INCREASING,			/*USED*/
	WHEEL_MIN,					/*NOT	USED, I REPEAT "NOT USED"*/
	WHEEL_0,					/*USED*/
	WHEEL_10,					/*USED*/
	WHEEL_20,					/*USED*/
	WHEEL_30,					/*USED*/
	WHEEL_40,					/*USED*/
	WHEEL_50,					/*USED*/
	WHEEL_60,					/*USED*/
	WHEEL_70,					/*USED*/
	WHEEL_80,					/*USED*/
	WHEEL_90,					/*USED*/
	WHEEL_100,					/*USED*/
	WHEEL_MAX,					/*NOT	USED, I REPEAT "NOT USED"*/
	WHEEL_RESET,				/*USED*/
	WHEEL_ENABLED,				/*NOT	USED*/
	WHEEL_DISABLED				/*NOT	USED*/
};

#endif