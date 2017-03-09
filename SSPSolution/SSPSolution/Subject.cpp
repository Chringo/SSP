#include "Subject.h"



Subject::Subject()
{
}


Subject::~Subject()
{
}

void Subject::Notify(unsigned int entityID, EVENT newEvent)
{
	std::map<unsigned int, Observer*>::iterator itr;

	for (itr = this->m_observers.begin(); itr != this->m_observers.end(); itr++)
	{
		itr->second->React(entityID, newEvent);
	}
}

void Subject::AddObserver(Observer * observer, unsigned int entityID)
{
	this->m_observers.insert( std::pair<unsigned int, Observer*>(entityID, observer) );
}

int Subject::RemoveObserver(unsigned int entityID)
{
	if (this->m_observers.erase(entityID) != 0)
	{
		return 1;
	}

	return 0;
}

//int Subject::RemoveObserver(Observer * observer)
//{
//	if (this->m_observers.erase(observer) != 0)
//	{
//		return 1;
//	}
//
//	return 0;
//}