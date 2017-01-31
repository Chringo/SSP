#include "LightHandler.h"
LIGHT::LightHandler::LightHandler()
{

}

LIGHT::LightHandler::~LightHandler()
{
}

void LIGHT::LightHandler::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->m_gDevice = nullptr;
	this->m_gDeviceContext = nullptr;
	this->m_LightVector.reserve(10);

	this->m_gDevice			 = device;
	this->m_gDeviceContext	 = deviceContext;
}

LIGHT::LightHandler* LIGHT::LightHandler::getInstance()
{
	static LightHandler instance;
	return &instance;
}

LIGHT::Light* LIGHT::LightHandler::Get_Light(unsigned int id)
{
	for (size_t i = 0; i < this->m_LightVector.size(); i++)
	{
		if (this->m_LightVector.at(i)->ID = id)
		{
			return this->m_LightVector.at(i);
		}
	}
}

void LIGHT::LightHandler::Add_Light(LIGHT::Light* light)
{
	this->m_LightVector.push_back(light);
}

void LIGHT::LightHandler::Remove_Light(unsigned int id)
{
	for (size_t i = 0; i < this->m_LightVector.size(); i++)
	{
		if (this->m_LightVector.at(i)->ID = id)
		{
			this->m_LightVector.erase(m_LightVector.begin() + i);
		}
	}
}
