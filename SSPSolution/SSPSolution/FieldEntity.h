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

	int Initialize(int entityID, Field* field);
	int Update(float deltaTime, InputHandler* inputHandler);
	int React(int entityID, EVENT reactEvent);
private:

};
#endif