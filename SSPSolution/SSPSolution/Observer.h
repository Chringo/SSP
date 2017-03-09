#ifndef SSPAPPLICATION_OBSERVERPATTERN_OBSERVER_H
#define SSPAPPLICATION_OBSERVERPATTERN_OBSERVER_H
#include "ComponentStructs.h"
class Observer
{
private:
protected:
public:
	Observer();
	virtual ~Observer();

	virtual int React(unsigned int entityID, EVENT reactEvent) = 0;
private:
};

#endif