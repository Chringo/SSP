#ifndef SSPAPPLICATION_DEBUG_DEBUGHANDLER_H
#define SSPAPPLICATION_DEBUG_DEBUGHANDLER_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <Psapi.h>
#include "ntverp.h"
#include <string>
#include "ComponentHandler.h"

#if VER_PRODUCTBUILD > 9600 //Has windows 10 sdk

#include <d3d11_3.h>
#include <dxgi1_4.h>

#pragma comment(lib, "dxgi.lib")

#endif

//Some guidelines to use the debug handler
// -Create timers before custom values
// -Create timer returns the id used to start and stop the timer

class DebugHandler
{
private:
	struct Timer {
		std::wstring label;
		TextComponent* textComp;
		unsigned int minTime;
		unsigned int maxTime;
		LARGE_INTEGER startTime;
		LARGE_INTEGER endTime;
		Timer()
		{
			this->minTime = 9999999;
			this->maxTime = 0;
		}
		~Timer()
		{
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
	struct Value {
		std::wstring label;
		TextComponent* textComp;
		float value;
	};

	// How many frames to use for average fps
	const static int m_FRAMES_FOR_AVG = 50;

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
	TextComponent* m_ramTextComp;
	TextComponent* m_vramTextComp;
	DWORDLONG m_totalPhysMem;

#if VER_PRODUCTBUILD > 9600 //Has windows 10 sdk

	IDXGIAdapter3* m_adapter;

#endif

	//int lol; //Needed to prevent heap corruption, don't ask why

	//static DebugHandler* m_instance;
	DebugHandler();

	void ClearConsole();

public:
	virtual ~DebugHandler();

	/*static DebugHandler& instance()
	{
		if (m_instance == nullptr) m_instance = new DebugHandler();
		return *m_instance;
	}*/
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