#include "LightHandler.h"
using namespace LIGHT;
LightHandler::LightHandler()
{
	Spot DukeKukem;
	Light* testing = new Light(DukeKukem);
}

LightHandler::~LightHandler()
{
}

void LightHandler::Initialize()
{
}

void LightHandler::Get_Light(unsigned int)
{
}

void LightHandler::Add_Light(unsigned int id, LightColor col, LightFalloff fall, Light light, LIGHT_TYPE lt)
{

	switch (lt)
	{
	case LIGHT::LT_POINT:
	{
		Point tempType;
		tempType.ID = id;
		tempType.color = col;
		tempType.LT = lt;
		tempType.falloff = fall;
		tempType.position = { 0,0,0 };
		tempType.radius = 1;
		Light* tempLight = new Light(tempType);
		this->m_LightVector.push_back(tempLight);
		break;
	}
	case LIGHT::LT_DIRECTIONAL:
	{
		Directional tempType;
		tempType.ID = id;
		tempType.color = col;
		tempType.LT = lt;
		tempType.direction = { 0,0,0 };
		tempType.intensity = 1;

		Light* tempLight = new Light(tempType);
		this->m_LightVector.push_back(tempLight);
		break;
	}
	case LIGHT::LT_AREA:
	{
		Area tempType;
		tempType.ID = id;
		tempType.color = col;
		tempType.LT = lt;
		tempType.direction = { 0,0,0 };
		tempType.intensity = 1;
		tempType.position = { 0,0,0 };
		Light* tempLight = new Light(tempType);
		this->m_LightVector.push_back(tempLight);
		break;
	}
	case LIGHT::LT_SPOT:
	{
		Spot tempType;
		tempType.ID = id;
		tempType.color = col;
		tempType.LT = lt;
		tempType.direction = { 0,0,0 };
		tempType.intensity = 1;
		tempType.position = { 0,0,0 };
		Light* tempLight = new Light(tempType);
		this->m_LightVector.push_back(tempLight);
		break;
	}
	default:
		break;
	}
}

void LightHandler::Remove_Light(unsigned int id)
{
}
