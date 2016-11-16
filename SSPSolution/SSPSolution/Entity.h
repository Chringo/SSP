#ifndef SSPAPPLICATION_ENTITIES_ENTITY_H
#define SSPAPPLICATION_ENTITIES_ENTITY_H
#include "Observer.h"
#include "Subject.h"

class Entity :
	public Observer
{
private:
protected:
	Subject* subject;
public:
	Entity();
	virtual ~Entity();

private:
};

#endif