#ifndef SSPAPPLICATION_OBSERVERPATTERN_SUBJECT_H
#define SSPAPPLICATION_OBSERVERPATTERN_SUBJECT_H
#include <map>
#include "Observer.h"
class Subject
{
private:
	std::map<unsigned int, Observer*> m_observers;	//Pair observer and EntityID
public:
	Subject();
	virtual ~Subject();

	void Notify(unsigned int entityID, EVENT newEvent);
	void AddObserver(Observer* observer, unsigned int entityID);
	int RemoveObserver(unsigned int entityID);
	//int RemoveObserver(Observer* observer);
private:
};

#endif