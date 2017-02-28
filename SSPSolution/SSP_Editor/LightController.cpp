#include "LightController.h"

void LightController::m_updateAmbient()
{
	LIGHTING::LightHandler::GetInstance()->SetAmbientLight
	(
		this->m_levelAmbience.r,
		this->m_levelAmbience.g,
		this->m_levelAmbience.b,
		this->m_levelAmbience.intensity
	);
}

LightController::LightController()
{

}

LightController::~LightController()
{

}

void LightController::Initialize()
{
	m_lights.reserve(100);
	pointLightData.reserve(100);

	m_updateAmbient();
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

void LightController::AddLight(Light* light, LIGHTING::Light * data, LIGHTING::LIGHT_TYPE type)
{

	switch (type)
	{
	case LIGHTING::LT_POINT:
	{
		Point* p = ((Point*)light);
		p->internalID = GlobalIDHandler::GetInstance()->GetNewId();
		p->isDirty = true;
		this->m_lights.push_back(p);
		this->pointLightData.push_back(*(LIGHTING::Point*)data);
		p->Initialize(&this->pointLightData.back());
		LIGHTING::LightHandler::GetInstance()->SetLightData(pointLightData.data(), pointLightData.size());
		//LIGHTING::LightHandler::GetInstance()->UpdateStructuredBuffer(type);
		break;
	}
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

void LightController::AddLight(LIGHTING::Point * light)
{
	this->pointLightData.push_back(*light);
	Point* p			  = new Point();
	p->data = &this->pointLightData.back();
	p->CreateFromExisting(light);
	p->internalID		  = GlobalIDHandler::GetInstance()->GetNewId();
	this->m_lights.push_back(p);
	LIGHTING::LightHandler::GetInstance()->SetLightData(pointLightData.data(), pointLightData.size());
	LIGHTING::LightHandler::GetInstance()->UpdateStructuredBuffer();

}

void LightController::AddLight(LIGHTING::LIGHT_TYPE type)
{
	Point * container = new Point();
	LIGHTING::Point data;

	switch (type)
	{
	case LIGHTING::LT_POINT:

		container->internalID = GlobalIDHandler::GetInstance()->GetNewId();
		container->isDirty    = true;
		this->m_lights.push_back(container);
		this->pointLightData.push_back(data);
		container->Initialize(&this->pointLightData.back());
		LIGHTING::LightHandler::GetInstance()->SetLightData(pointLightData.data(), pointLightData.size());
		LIGHTING::LightHandler::GetInstance()->UpdateStructuredBuffer();
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
	LIGHTING::LightHandler::GetInstance()->UpdateStructuredBuffer();
}

void LightController::RemoveLight(int index, LIGHTING::LIGHT_TYPE type)
{
	//gonna need to find the data index here aswell if we want support for more lighttypes
	//for now i assume they're the same
	switch (type)
	{
	case LIGHTING::LT_POINT:
		m_lights.erase(m_lights.begin() + index);
		pointLightData.erase(pointLightData.begin() + index);
		LIGHTING::LightHandler::GetInstance()->SetLightData(pointLightData.data(), pointLightData.size());
		LIGHTING::LightHandler::GetInstance()->UpdateStructuredBuffer();

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

void LightController::SetAmbientR(float r)
{
	this->m_levelAmbience.r = r;
	m_updateAmbient();
}

void LightController::SetAmbientG(float g)
{
	this->m_levelAmbience.g = g;
	m_updateAmbient();
}

void LightController::SetAmbientB(float b)
{
	this->m_levelAmbience.b = b;
	m_updateAmbient();
}

void LightController::SetAmbientIntensity(float intensity)
{
	this->m_levelAmbience.intensity = intensity;
	m_updateAmbient();
}

void LightController::SetLevelAmbient(Ambient ambient)
{
	this->m_levelAmbience = ambient;
	//Ui::AttributesHandler::GetInstance()->SetAmbientLight(ambient);
	m_updateAmbient();
}

void LightController::DisplayLightRadius(bool display)
{
	m_displayLightRadius = display;
}

bool LightController::DisplayLightRadius()
{
	return m_displayLightRadius;
}

std::vector<int>* LightController::GetShadowCasterIndexList()
{
	return &shadowCasterIndexes;
}

void LightController::MakeShadowCaster(unsigned int internalID)
{
	for (int i = 0; i < m_lights.size(); i++)
	{
		if (m_lights[i]->internalID == internalID)
		{
			for (int shadowIndex : shadowCasterIndexes)
				if (shadowIndex == i)
					return;
			shadowCasterIndexes.push_back(i);
			return;
		}
	}
}

void LightController::RemoveShadowCaster(unsigned int internalID)
{
	for (int i = 0; i < m_lights.size(); i++)
		if (m_lights[i]->internalID == internalID)
			for (int j = 0; j < shadowCasterIndexes.size(); j++)
				if (shadowCasterIndexes.at(j) == i)
				{
					shadowCasterIndexes.erase(shadowCasterIndexes.begin() + j);
					return;
				}
	return;
}

bool LightController::GetIsShadowCaster(unsigned int internalID)
{

	for (int i = 0; i < m_lights.size(); i++)
	{
		if (m_lights[i]->internalID == internalID)
		{
			for (int shadowCaster : shadowCasterIndexes)
			{
				if (i == shadowCaster)
					return true;
			}
			return false;
		}
	}
	return false;
}

void LightController::Destroy()
{
	for each (Light* container in m_lights)
	{
		delete container;
	}
	this->GetLights()->clear();
	this->pointLightData.clear();
	LIGHTING::LightHandler::GetInstance()->SetLightData(pointLightData.data(), pointLightData.size());
	LIGHTING::LightHandler::GetInstance()->UpdateStructuredBuffer();

}
