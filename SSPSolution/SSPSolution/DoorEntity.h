#ifndef SSPAPPLICATION_ENTITIES_DOORENTITY_H
#define SSPAPPLICATION_ENTITIES_DOORENTITY_H
#include "Entity.h"
#include <vector>

struct ElementState {
	unsigned int entityID;
	EVENT desiredState;
	bool desiredStateReached;
};

class DoorEntity :
	public Entity
{
private:

	std::vector<ElementState> m_subjectStates;
	
	DirectX::XMMATRIX m_originalOrto;
	DirectX::XMVECTOR m_closed_pos;

	bool m_isOpened;
	float m_minRotation;
	float m_maxRotation;
	float m_rotateTime;
	float m_rotatePerSec;
	bool m_needSync;

	DirectX::XMVECTOR pivotPoint;

	float m_animSpeed = 70.0f;
	float m_targetRot = 0;
	float m_currRot   = 0;
	bool m_animationActive = false;
	float m_activatedRotation = 90.0f;
public:
	DoorEntity();
	virtual ~DoorEntity();
	
	int Initialize(int entityID, PhysicsComponent* pComp, GraphicsComponent* gComp, std::vector<ElementState> subjectStates, float rotateTime = 1.0f, float minRotation = 0.0f, float maxRotation = DirectX::XM_PI / 2.0f);

	int Update(float dT, InputHandler* inputHandler);
	int React(int entityID, EVENT reactEvent);

	bool SetIsOpened(bool isOpened);
	bool GetIsOpened();

	bool AddSubjectState(ElementState subjectState);
	bool AddSubjectState(unsigned int entityID, EVENT requiredEvent);
	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };
};

#endif