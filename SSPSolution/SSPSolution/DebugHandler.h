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
		LARGE_INTEGER startTime;
		LARGE_INTEGER endTime;
		Timer()
		{
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
			return (unsigned int)(elapsedTime.QuadPart / frequency.QuadPart);
		}
	};

	LARGE_INTEGER m_frequency;
	std::vector<Timer> m_timers;
	std::vector<std::string> m_labels;
	std::vector<unsigned int> m_timerMins;
	std::vector<unsigned int> m_timerMaxs;
	std::vector<std::string> m_labelsValues;
	std::vector<float> m_customValues;
	unsigned int m_frameTimes[FRAMES_FOR_AVG];
	unsigned int m_maxFPS;
	unsigned int m_minFPS;
	int m_currFrameTimesPtr;
	unsigned short int m_timerToEnd;
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

	int StartTimer(std::string label); //returns timer ID, -1 fail
	int EndTimer();
	int EndTimer(int timerID);

	int StartProgram(); // Needed at start of program to show timer %
	int EndProgram(); // Needed at end of program to show timer %
	int ShowFPS(bool show);

	int CreateCustomLabel(std::string label, float value); //returns label ID, -1 fail
	int UpdateCustomLabel(int labelID, float newValue);
	int UpdateCustomLabelIncrease(int labelID, float addValue);

	int ResetMinMax();

	int Display(float dTime); // Call this in the end to display everything in the console

	void Shutdown();
};

#endif