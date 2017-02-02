#include "LightController.h"

LightController::LightController()
{

}

LightController::~LightController()
{

}

void LightController::Initialize()
{
	
}

LightController * LightController::GetInstance()
{
	static LightController lightC;
	return &lightC;
}

void LightController::synchData(LIGHTING::LIGHT_TYPE type)
{
	switch (type)
	{
	case LIGHTING::LT_POINT:
		break;
	case LIGHTING::LT_DIRECTIONAL:
		break;
	case LIGHTING::LT_AREA:
		break;
	case LIGHTING::LT_SPOT:
		break;
	case LIGHTING::NUM_LT:
		break;
	default:
		break;
	}
}

void LightController::AddLight(LIGHTING::LIGHT_TYPE type)
{
	Point * container = new Point();
	LIGHTING::Point data;

	container->Initialize(&data);
	switch (type)
	{
	case LIGHTING::LT_POINT:

		container->internalID = GlobalIDHandler::GetInstance()->GetNewId();
		container->isDirty = true;
		this->m_lights.push_back(container);
		this->pointLightData.push_back(data);

		break;
	case LIGHTING::LT_DIRECTIONAL:
		break;
	case LIGHTING::LT_AREA:
		break;
	case LIGHTING::LT_SPOT:
		break;
	case LIGHTING::NUM_LT:
		break;
	default:
		break;
	}
}

void LightController::UpdateLights(LIGHTING::LIGHT_TYPE type)
{
	LIGHTING::LightHandler::GetInstance()->UpdateStructuredBuffer(type);
}
