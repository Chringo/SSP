#ifndef SSPAPPLICATION_OBSERVERPATTERN_SUBJECT_H
#define SSPAPPLICATION_OBSERVERPATTERN_SUBJECT_H
#include <vector>
#include "Observer.h"
class Subject
{
private:
	std::vector<std::pair<Observer*, int>> m_observers;
public:
	Subject();
	virtual ~Subject();

	int Notify(int entityID, EVENT newEvent);
	int AddObserver(Observer* observer);
	int RemoveObserver(Observer* observer);
	int RemoveObserver(int entityID);
private:
};

#endif