#ifndef SSPEDITOR_LIGHTCONTROLLER_H
#define SSPEDITOR_LIGHTCONTROLLER_H
#include "Header.h"
#include "GlobalIDHandler.h"
class LightController
{
public:
	~LightController();
	void Initialize();
	static LightController *	  GetInstance();
	std::vector<Light*> *		  GetLights() { return &m_lights; };
	std::vector<LIGHTING::Point>* GetPointLightData() { return &pointLightData; };
	void synchData(LIGHTING::LIGHT_TYPE type);
	void AddLight(LIGHTING::LIGHT_TYPE type);
	void AddLight(Light* light, LIGHTING::Light * data, LIGHTING::LIGHT_TYPE type);
	void AddLight(LIGHTING::Point* light);
	void UpdateLights(LIGHTING::LIGHT_TYPE type);
	void RemoveLight(int index, LIGHTING::LIGHT_TYPE type);
	void Destroy();
private:
	LightController();
	std::vector<Light*> m_lights;
	std::vector<LIGHTING::Point> pointLightData;


};




#endif