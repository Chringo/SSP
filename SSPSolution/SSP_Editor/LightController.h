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
	void UpdateLights(LIGHTING::LIGHT_TYPE type);
	void RemoveLight(int index, LIGHTING::LIGHT_TYPE type);
	void SetAmbientR(float r);
	void SetAmbientG(float g);
	void SetAmbientB(float b);
	void SetAmbientIntensity(float intensity);
	void SetLevelAmbient(Ambient ambient);
	const Ambient * GetLevelAmbient() { return &this->m_levelAmbience; };
	void Destroy();
private:
	void m_updateAmbient();
	LightController();
	Ambient m_levelAmbience = Ambient();
	std::vector<Light*> m_lights;
	std::vector<LIGHTING::Point> pointLightData;


};




#endif