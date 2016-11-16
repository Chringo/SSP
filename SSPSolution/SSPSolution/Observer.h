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

	virtual int React(int entityID, EVENT event) = 0;
private:
};

#endif