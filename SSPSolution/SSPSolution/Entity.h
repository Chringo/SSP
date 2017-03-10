#ifndef SSPAPPLICATION_ENTITIES_ENTITY_H
#define SSPAPPLICATION_ENTITIES_ENTITY_H
#include "Observer.h"
#include "Subject.h"
#include "InputHandler.h"
//Subject includes this for the events
#include "../GraphicsDLL/GraphicsComponent.h"
#include "../physicsDLL/PhysicsHandler.h"
#include "../AIDLL/AIComponent.h"
#include "ComponentHandler.h"
class Entity :
	public Observer
{
private:
	//Variables
protected:
	unsigned int m_entityID;
	bool m_isGrabbed;
	Entity* m_isGrabbedBy;
	Subject m_subject;
	PhysicsComponent* m_pComp;
	GraphicsComponent* m_gComp;
	AIComponent* m_aiComp;
	AnimationComponent* m_aComp;

public:
	Entity();
	virtual ~Entity();

	//dT should be in seconds
	virtual int Update(float dT, InputHandler* inputHandler) = 0;
	virtual int React(unsigned int entityID, EVENT reactEvent) = 0;
	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };

	//Returns 1 if synchronization was needed, -1 if physicscomponent was missing, -2 if graphicscomponent was missing, -3 if both components were missing.
	int SyncComponents();
	int AddObserver(Observer* observer, unsigned int entityID);


	PhysicsComponent* SetPhysicsComponent(PhysicsComponent* pComp);
	GraphicsComponent* SetGraphicsComponent(GraphicsComponent* gComp);
	AIComponent* SetAIComponent(AIComponent* aiComp);
	AnimationComponent* SetAnimationComponent(AnimationComponent* aComp);
	bool SetGrabbed(Entity* isGrabbedBy);
	bool IsGrabbed();
	unsigned int SetEntityID(unsigned int entityID);

	PhysicsComponent* GetPhysicsComponent();
	GraphicsComponent* GetGraphicComponent();
	AIComponent* GetAIComponent();
	AnimationComponent* GetAnimationComponent();
	Entity* GetISGrabbedBy();

	bool GetGrabbed();
	unsigned int GetEntityID();
private:
	//Functions
protected:
	void UnsafeSyncComponents();
	//Returns 1 for correct and 0 for incorrect initialization.
	int InitializeBase(unsigned int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp, AnimationComponent* aComp, AIComponent* aiComp = nullptr);

};

#endif