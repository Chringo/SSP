#include "DebugHandler.h"

DebugHandler* DebugHandler::m_instance = nullptr;

DebugHandler::DebugHandler()
{
	QueryPerformanceFrequency(&this->m_frequency);
	this->m_displayFPS = true;
	this->ClearConsole();
	for (int i = 0; i < FRAMES_FOR_AVG; i++)
	{
		this->m_frameTimes[i] = 40;
	}
	this->m_minFPS = 999999;
	this->m_maxFPS = 0;
}

void DebugHandler::ClearConsole()
{
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}


DebugHandler::~DebugHandler()
{
}

int DebugHandler::StartTimer(int timerID)
{
	int result = 0;

	if (timerID < this->m_timers.size())
	{
		LARGE_INTEGER currTime;
		QueryPerformanceCounter(&currTime);
		this->m_timers.at(timerID).startTime = currTime;
		result = 1;
	}

	return result;
}

int DebugHandler::EndTimer(int timerID)
{
	int result = 0;

	if (timerID < this->m_timers.size())
	{
		LARGE_INTEGER currTime;
		QueryPerformanceCounter(&currTime);
		this->m_timers.at(timerID).endTime = currTime;
		result = 1;
	}

	return result;
}

int DebugHandler::CreateTimer(std::string label)
{
	Timer timer;
	timer.label = label;
	this->m_timers.push_back(timer);

	return 0;
}

int DebugHandler::StartProgram()
{
	QueryPerformanceCounter(&this->m_programStart);

	return 0;
}

int DebugHandler::EndProgram()
{
	QueryPerformanceCounter(&this->m_programEnd);

	return 0;
}

int DebugHandler::ShowFPS(bool show)
{
	this->m_displayFPS = show;

	return 0;
}

int DebugHandler::CreateCustomLabel(std::string label, float value)
{
	this->m_labelsValues.push_back(label);
	this->m_customValues.push_back(value);

	return 0;
}

int DebugHandler::UpdateCustomLabel(int labelID, float newValue)
{
	if ((unsigned int)labelID < this->m_labelsValues.size())
	{
		this->m_customValues.at(labelID) = newValue;
	}
	else 
	{
		return -1;
	}

	return 0;
}

int DebugHandler::UpdateCustomLabelIncrease(int labelID, float addValue)
{
	if ((unsigned int)labelID < this->m_labelsValues.size())
	{
		this->m_customValues.at(labelID) += addValue;
	}
	else
	{
		return -1;
	}

	return 0;
}

int DebugHandler::ResetMinMax()
{
	std::vector<Timer>::iterator iter;

	for (iter = this->m_timers.begin(); iter != this->m_timers.end(); iter++)
	{
		iter->maxTime = 0;
		iter->minTime = 999999;
	}

	this->m_minFPS = 999999;
	this->m_maxFPS = 0;

	return 0;
}

int DebugHandler::DisplayConsole(float dTime)
{
	COORD topLeft = { 0, 0 };
	COORD FPSLocation = { 50, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	//GetConsoleScreenBufferInfo(console, &screen);
	/*FillConsoleOutputCharacterA(
		console, ' ', toClear.X * toClear.Y, topLeft, &written
	);*/
	/*FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		toClear.X * toClear.Y, topLeft, &written
	);*/
	SetConsoleCursorPosition(console, topLeft);

	std::vector<Timer>::iterator iter;

	unsigned int time, minTime, maxTime;
	int i;
	for (i = 0, iter = this->m_timers.begin();
		iter != this->m_timers.end();
		i++, iter++)
	{
		time = iter->GetTimeMS(this->m_frequency);

		minTime = this->m_timers.at(i).minTime;
		maxTime = this->m_timers.at(i).maxTime;
		this->m_timers.at(i).minTime = (minTime < time) ? minTime : time;
		this->m_timers.at(i).maxTime = (maxTime > time) ? maxTime : time;

		LARGE_INTEGER elapsedTime;
		elapsedTime.QuadPart = this->m_programEnd.QuadPart - this->m_programStart.QuadPart;
		elapsedTime.QuadPart *= 1000000;
		elapsedTime.QuadPart /= this->m_frequency.QuadPart;

		std::cout << std::fixed << std::setprecision(1) << iter->label.c_str() << ": [" << iter->minTime << "] "
			<< time << " [" << iter->maxTime << "] us, " 
			<< (float)((time / (float)elapsedTime.QuadPart) * 100) << "%";
		GetConsoleScreenBufferInfo(console, &screen);
		FillConsoleOutputCharacterA(
			console, ' ', 5, screen.dwCursorPosition, &written
		);
		std::cout << std::endl;
	}
	int nrOfCustomLabels = this->m_labelsValues.size();
	for (int j = 0; j < nrOfCustomLabels; j++)
	{
		std::cout << this->m_labelsValues.at(j) << ": " << this->m_customValues.at(j);
		GetConsoleScreenBufferInfo(console, &screen);
		FillConsoleOutputCharacterA(
			console, ' ', 5, screen.dwCursorPosition, &written
		);
		std::cout << std::endl;
	}

	if (this->m_displayFPS)
	{
		int sum = 0, avgFPS;
		this->m_currFrameTimesPtr = (this->m_currFrameTimesPtr >= FRAMES_FOR_AVG) ? 0 : this->m_currFrameTimesPtr;
		this->m_frameTimes[this->m_currFrameTimesPtr] = (unsigned int)(1000000 / dTime);
		for (int k = 0; k < FRAMES_FOR_AVG; k++)
		{
			sum += this->m_frameTimes[k];
		}
		avgFPS = sum / FRAMES_FOR_AVG;
		this->m_minFPS = (this->m_minFPS < this->m_frameTimes[this->m_currFrameTimesPtr]) ? this->m_minFPS : this->m_frameTimes[this->m_currFrameTimesPtr];
		this->m_maxFPS = (this->m_maxFPS > this->m_frameTimes[this->m_currFrameTimesPtr]) ? this->m_maxFPS : this->m_frameTimes[this->m_currFrameTimesPtr];
		SetConsoleCursorPosition(console, FPSLocation);
		std::cout << "FPS: " << avgFPS << " [" << this->m_minFPS << "] (" << std::to_string(this->m_frameTimes[this->m_currFrameTimesPtr]) << ") [" << this->m_maxFPS << "]";
		GetConsoleScreenBufferInfo(console, &screen);
		FillConsoleOutputCharacterA(
			console, ' ', 8, screen.dwCursorPosition, &written
		);
		this->m_currFrameTimesPtr++;
		
	}

	COORD finishedCursonLoc = { 0, this->m_timers.size() + nrOfCustomLabels + 1 };
	SetConsoleCursorPosition(console, finishedCursonLoc);

	return 0;
}

void DebugHandler::Shutdown()
{
	if (m_instance != nullptr)
	{
		delete this->m_instance;
	}
}
