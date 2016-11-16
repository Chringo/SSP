#include "Subject.h"



Subject::Subject()
{
}


Subject::~Subject()
{
}

int Subject::Notify(int entityID, EVENT newEvent)
{
	return 0;
}

int Subject::AddObserver(Observer * observer)
{
	return 0;
}

int Subject::RemoveObserver(Observer * observer)
{
	return 0;
}

int Subject::RemoveObserver(int entityID)
{
	return 0;
}
