#ifndef SSPAPPLICATION_DEBUG_DEBUGHANDLER_H
#define SSPAPPLICATION_DEBUG_DEBUGHANDLER_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <string>

// How many frames to use for average fps
const int FRAMES_FOR_AVG = 100;

class DebugHandler
{
private:
	struct Timer {
		std::string label;
		unsigned int minTime;
		unsigned int maxTime;
		LARGE_INTEGER startTime;
		LARGE_INTEGER endTime;
		Timer()
		{
			this->label = "EMPTY";
			this->minTime = 9999999;
			this->maxTime = 0;
		}
		~Timer()
		{
		}
		Timer(LARGE_INTEGER start)
		{
			this->startTime = start;
		}
		unsigned int GetTimeMS(LARGE_INTEGER frequency) // !!! Not MS atm  us
		{
			LARGE_INTEGER elapsedTime;
			elapsedTime.QuadPart = this->endTime.QuadPart - this->startTime.QuadPart;
			elapsedTime.QuadPart *= 1000000;
			unsigned int time = (unsigned int)(elapsedTime.QuadPart / frequency.QuadPart);
			minTime = (minTime < time) ? minTime : time;
			maxTime = (maxTime > time) ? maxTime : time;
			return time;
		}
	};

	LARGE_INTEGER m_frequency;
	std::vector<Timer> m_timers;
	std::vector<std::string> m_labelsValues;
	std::vector<float> m_customValues;
	unsigned int m_frameTimes[FRAMES_FOR_AVG];
	unsigned int m_maxFPS;
	unsigned int m_minFPS;
	int m_currFrameTimesPtr;
	LARGE_INTEGER m_programStart;
	LARGE_INTEGER m_programEnd;
	bool m_displayFPS;

	static DebugHandler* m_instance;
	DebugHandler();

	void ClearConsole();

public:
	~DebugHandler();

	static DebugHandler& instance()
	{
		if (m_instance == nullptr) m_instance = new DebugHandler();
		return *m_instance;
	}

	int CreateTimer(std::string label); //returns timer ID, -1 fail
	int StartTimer(int timerID); 
	int EndTimer(int timerID); 

	int StartProgram(); // Needed at start of program to show timer %
	int EndProgram(); // Needed at end of program to show timer %
	int ShowFPS(bool show);

	int CreateCustomLabel(std::string label, float value); //returns label ID, -1 fail
	int UpdateCustomLabel(int labelID, float newValue);
	int UpdateCustomLabelIncrease(int labelID, float addValue);

	int ResetMinMax();

	int DisplayConsole(float dTime); // Call this in the end to display everything in the console
	int Display();

	void Shutdown();
};

#endif