#ifndef SSPAPPLICATION_DEBUG_DEBUGHANDLER_H
#define SSPAPPLICATION_DEBUG_DEBUGHANDLER_H

#include <vector>
#include <iostream>
#include <Windows.h>

class DebugHandler
{
private:
	std::vector<LARGE_INTEGER> m_timers;
	std::vector<std::string> m_labels;
	std::vector<unsigned short int> m_timerMins;
	std::vector<unsigned short int> m_timerMaxs;
	std::vector<float> m_customValues;
	unsigned short int m_lastFPS[10];
	unsigned short int m_lastFPSCurr;

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