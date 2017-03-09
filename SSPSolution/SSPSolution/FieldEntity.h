#ifndef SSPAPPLICATION_ENTITIES_FIELDENTITY_H
#define SSPAPPLICATION_ENTITIES_FIELDENTITY_H
#include "Entity.h"
class FieldEntity :
	public Entity
{
private:
	//unsigned int m_checkpointNumber;// Should be a pointer, see checkpointNumber?
	Field* m_field;
public:
	FieldEntity();
	~FieldEntity();

	int Initialize(unsigned int entityID, Field* field);
	int Update(float deltaTime, InputHandler* inputHandler);
	int React(unsigned int entityID, EVENT reactEvent);

	Field* GetField();
private:
};
#endif