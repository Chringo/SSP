#ifndef SSPAPPLICATION_ENTITIES_ENTITY_H
#define SSPAPPLICATION_ENTITIES_ENTITY_H
#include "Observer.h"
#include "Subject.h"
#include "InputHandler.h"
//Subject includes this for the events
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../physicsDLL/PhysicsHandler.h"
#include "ComponentHandler.h"
class Entity :
	public Observer
{
private:
	//Variables
protected:
	int m_entityID;
	bool m_isGrabbed;
	Subject m_subject;
	PhysicsComponent* m_pComp;
	GraphicsComponent* m_gComp;

public:
	Entity();
	virtual ~Entity();

	//dT should be in seconds
	virtual int Update(float dT, InputHandler* inputHandler) = 0;
	virtual int React(int entityID, EVENT reactEvent) = 0;

	PhysicsComponent* SetPhysicsComponent(PhysicsComponent* pComp);
	GraphicsComponent* SetGraphicsComponent(GraphicsComponent* gComp);
	bool SetGrabbed(int isGrabbed);
	int SetEntityID(int entityID);

	PhysicsComponent* GetPhysicsComponent();
	GraphicsComponent* GetGraphicComponent();
	bool GetGrabbed();
	int GetEntityID();
private:
	//Functions
protected:
	//Returns 1 for correct and 0 for incorrect initialization.
	int InitializeBase(int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp);

};

#endif