#ifndef SSPAPPLICATION_OBSERVERPATTERN_SUBJECT_H
#define SSPAPPLICATION_OBSERVERPATTERN_SUBJECT_H
#include <map>
#include "Observer.h"
class Subject
{
private:
	std::map<int, Observer*> m_observers;	//Pair observer and EntityID
public:
	Subject();
	virtual ~Subject();

	void Notify(int entityID, EVENT newEvent);
	void AddObserver(Observer* observer, int entityID);
	int RemoveObserver(int entityID);
	//int RemoveObserver(Observer* observer);
private:
};

#endif