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
	void UpdateShadows();
	void RemoveLight(int index, LIGHTING::LIGHT_TYPE type);
	void SetAmbientR(float r);
	void SetAmbientG(float g);
	void SetAmbientB(float b);
	void SetAmbientIntensity(float intensity);
	void SetLevelAmbient(Ambient ambient);
	void SetGraphicsHandler(GraphicsHandler* gh) { this->gh = gh; };
	const Ambient * GetLevelAmbient() { return &this->m_levelAmbience; };
	void DisplayLightRadius(bool display);
	bool DisplayLightRadius();
	std::vector<int> * GetShadowCasterIndexList();
	void MakeShadowCaster(unsigned int internalID);
	void RemoveShadowCaster(unsigned int internalID);
	bool GetIsShadowCaster(unsigned int internalID);


	void Destroy();
private:
	void m_updateAmbient();
	LightController();
	Ambient m_levelAmbience = Ambient();
	std::vector<Light*> m_lights;
	std::vector<LIGHTING::Point> pointLightData;
	std::vector<int> shadowCasterIndexes;
	bool m_displayLightRadius;
	GraphicsHandler* gh = nullptr;

};




#endif