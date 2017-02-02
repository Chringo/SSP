#ifndef SSPEDITOR_LIGHTCONTROLLER_H
#define SSPEDITOR_LIGHTCONTROLLER_H
#include "Header.h"
#include "../GraphicsDLL/LightHandler.h"
#include "GlobalIDHandler.h"
class LightController
{
public:
	~LightController();
	void Initialize();
	static LightController * GetInstance();
	std::vector<Light*> * GetLights() { return &m_lights; };
	void synchData(LIGHTING::LIGHT_TYPE type);
	void AddLight(LIGHTING::LIGHT_TYPE type);
	void UpdateLights(LIGHTING::LIGHT_TYPE type);
private:
	LightController();
	std::vector<Light*> m_lights;
	std::vector<LIGHTING::Point> pointLightData;


};




#endif