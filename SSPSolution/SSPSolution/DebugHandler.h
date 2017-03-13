#ifndef SSPAPPLICATION_DEBUG_DEBUGHANDLER_H
#define SSPAPPLICATION_DEBUG_DEBUGHANDLER_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <Psapi.h>
#include <string>
#include "ComponentHandler.h"
#include <d3d11_3.h>
#include <dxgi1_4.h>

#pragma comment(lib, "dxgi.lib")


//Some guidelines to use the debug handler
// -Create timers before custom values
// -Create timer returns the id used to start and stop the timer
// -ComponentHandler needs to be set before usage!
// -Layouts for timers and FPS:
//	+[MinTime] CurrentTime [MaxTime] (AvgTime) Curr% (Avg%)
//  +FPS AvgFPS [MinFPS] CurrentFPS [MaxFPS]

class DebugHandler
{
private:
	// How many frames to use for average fps
	const static int m_FRAMES_FOR_AVG = 100;

	struct Timer {
		std::wstring label;
		TextComponent* textComp;
		unsigned int minTime;
		unsigned int maxTime;
		LARGE_INTEGER startTime;
		LARGE_INTEGER endTime;
		unsigned int avgPercentage[m_FRAMES_FOR_AVG];
		unsigned int avgTime[m_FRAMES_FOR_AVG];
		int currAvgPtr;
		Timer()
		{
			this->minTime = 9999999;
			this->maxTime = 0;
			this->currAvgPtr = 0;
			for (int i = 0; i < m_FRAMES_FOR_AVG; i++)
			{
				this->avgPercentage[i] = 10;
				this->avgTime[i] = 1000;
			}
		}
		~Timer()
		{
		}
		unsigned int GetTimeMS(LARGE_INTEGER frequency, LARGE_INTEGER elaspedTime) // !!! Not MS atm  us
		{
			LARGE_INTEGER elapsedTimeLocal;
			elapsedTimeLocal.QuadPart = this->endTime.QuadPart - this->startTime.QuadPart;
			elapsedTimeLocal.QuadPart *= 1000000;
			unsigned int time = (unsigned int)(elapsedTimeLocal.QuadPart / frequency.QuadPart);
			minTime = (minTime < time) ? minTime : time;
			maxTime = (maxTime > time) ? maxTime : time;
			avgPercentage[currAvgPtr] = (unsigned int)((time / (float)elaspedTime.QuadPart) * 100);
			avgTime[currAvgPtr] = time;
			return time;
		}
		unsigned int GetAvgTime()
		{
			unsigned int sum = 0;
			for (int i = 0; i < m_FRAMES_FOR_AVG; i++)
			{
				sum += this->avgTime[i];
			}
			return sum / m_FRAMES_FOR_AVG;
		}
		unsigned int GetAvgPercentage()
		{
			unsigned int sum = 0;
			for (int i = 0; i < m_FRAMES_FOR_AVG; i++)
			{
				sum += this->avgPercentage[i];
			}
			currAvgPtr = (currAvgPtr >= m_FRAMES_FOR_AVG) ? 0 : currAvgPtr+1;
			return sum / m_FRAMES_FOR_AVG;
		}
	};
	struct Value {
		std::wstring label;
		TextComponent* textComp;
		float value;
	};

	LARGE_INTEGER m_frequency;
	std::vector<Timer> m_timers;
	std::vector<Value> m_values;
	unsigned int m_frameTimes[m_FRAMES_FOR_AVG];
	unsigned int m_maxFPS;
	unsigned int m_minFPS;
	int m_currFrameTimesPtr;
	TextComponent* m_fpsTextComp;
	LARGE_INTEGER m_programStart;
	LARGE_INTEGER m_programEnd;
	bool m_displayDebug;
	ComponentHandler* compHandler;
	TextComponent* m_physRamTextComp;
	TextComponent* m_virtRamTextComp;
	TextComponent* m_pageFaultTextComp;
	TextComponent* m_vramTextComp;
	IDXGIAdapter3* m_adapter;

	DebugHandler();

	void ClearConsole();

public:
	virtual ~DebugHandler();

	static DebugHandler* instance();

	int SetComponentHandler(ComponentHandler* compHandler);

	int CreateTimer(std::wstring label); //returns timer ID, -1 fail
	int StartTimer(size_t timerID);
	int EndTimer(size_t timerID);

	int StartProgram(); // Needed at start of program to show timer %
	int EndProgram(); // Needed at end of program to show timer %
	int ToggleDebugInfo();

	int CreateCustomLabel(std::wstring label, float value); //returns label ID, -1 fail
	int UpdateCustomLabel(int labelID, float newValue);
	int UpdateCustomLabelIncrease(int labelID, float addValue);

	int ResetMinMax();

	int DisplayConsole(float dTime); // Call this in the end to display everything in the console
	int DisplayOnScreen(float dTime);

	void Shutdown();
};

#endif