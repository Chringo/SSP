#ifndef SSPAPPLICATION_DEBUG_DEBUGHANDLER_H
#define SSPAPPLICATION_DEBUG_DEBUGHANDLER_H

#include <vector>
#include <iostream>
#include <Windows.h>

class DebugHandler
{
private:
	struct Timer {
		LARGE_INTEGER startTime;
		LARGE_INTEGER endTime;
		Timer();
		Timer(LARGE_INTEGER start)
		{
			this->startTime = start;
		}
		unsigned int GetTimeMS(LARGE_INTEGER frequency)
		{
			LARGE_INTEGER elapsedTime;
			elapsedTime.QuadPart = this->startTime.QuadPart - this->endTime.QuadPart;
			elapsedTime.QuadPart *= 1000000;
			return elapsedTime.QuadPart / frequency.QuadPart;
		}
	};

	LARGE_INTEGER m_frequency;
	std::vector<Timer> m_timers;
	std::vector<std::string> m_labels;
	std::vector<unsigned short int> m_timerMins;
	std::vector<unsigned short int> m_timerMaxs;
	std::vector<float> m_customValues;
	unsigned short int m_frameTimes[10];
	unsigned short int m_currFrameTimesPtr;
	unsigned short int m_timerToEnd;

public:
	DebugHandler();
	~DebugHandler();

	int StartTimer(std::string label); //returns timer ID, -1 fail
	int EndTimer();
	int EndTimer(int timerID);

	int StartProgram();
	int EndProgram();
	int ShowFPS(bool show);

	int CreateCustomLabel(std::string label, float value); //returns label ID, -1 fail
	int UpdateCustomLabel(int labelID, float newValue);

	int Display();
};

#endif