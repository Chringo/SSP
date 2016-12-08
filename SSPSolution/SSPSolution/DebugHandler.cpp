#include "DebugHandler.h"

DebugHandler* DebugHandler::m_instance = nullptr;

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
	this->m_timers.at(this->m_timers.size() - 1).endTime = currTime;

	return 0;
}

int DebugHandler::Display()
{
	system("cls");

	std::vector<Timer>::iterator iter;
	std::vector<std::string>::iterator iterLabel;
	for (iter = this->m_timers.begin(), iterLabel = this->m_labels.begin(); 
		iter != this->m_timers.end() && iterLabel != this->m_labels.end();
		iter++, iterLabel++)
	{
		std::cout << iterLabel->c_str() << ": " << iter->GetTimeMS(this->m_frequency) << " ms" << std::endl;
	}

	this->m_timers.clear();
	this->m_labels.clear();
	this->m_timerToEnd = 0;

	return 0;
}

void DebugHandler::Shutdown()
{
	if (m_instance != nullptr) delete this->m_instance;
}
