#include "DebugHandler.h"

DebugHandler::DebugHandler()
{
	QueryPerformanceFrequency(&this->m_frequency);
	this->m_timerToEnd = 0;
}


DebugHandler::~DebugHandler()
{
}

int DebugHandler::StartTimer(std::string label)
{
	LARGE_INTEGER currTime;
	int result = this->m_timers.size();
	QueryPerformanceCounter(&currTime);
	this->m_timers.push_back(Timer(currTime));
	this->m_labels.push_back(label);

	return result;
}

int DebugHandler::EndTimer()
{
	LARGE_INTEGER currTime;
	QueryPerformanceCounter(&currTime);
	this->m_timers.at(this->m_timers.size() - this->m_timerToEnd++).endTime = currTime;

	return 0;
}
