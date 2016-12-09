#include "DebugHandler.h"

DebugHandler* DebugHandler::m_instance = nullptr;

DebugHandler::DebugHandler()
{
	QueryPerformanceFrequency(&this->m_frequency);
	this->m_timerToEnd = 0;
	this->m_displayFPS = true;
	this->ClearConsole();
	for (int i = 0; i < FRAMES_FOR_AVG; i++)
	{
		this->m_frameTimes[i] = 40;
	}
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

int DebugHandler::StartTimer(std::string label)
{
	LARGE_INTEGER currTime;
	QueryPerformanceCounter(&currTime);
	int result = this->m_timers.size();
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
	if (labelID < this->m_labelsValues.size())
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
	if (labelID < this->m_labelsValues.size())
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
	this->m_timerMins.clear();
	this->m_timerMaxs.clear();

	return 0;
}

int DebugHandler::Display(float dTime)
{
	COORD topLeft = { 0, 0 };
	COORD FPSLocation = { 100, 0 };
	//COORD toClear = { 4, 100 };
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
	std::vector<std::string>::iterator iterLabel;
	int nrOfTimers = this->m_timers.size();
	for (int i = this->m_timerMins.size(); i < nrOfTimers; i++)
	{
		this->m_timerMins.push_back(9999999);
		this->m_timerMaxs.push_back(0);
	}

	unsigned int time, minTime, maxTime;
	int i;
	for (i = 0, iter = this->m_timers.begin(), iterLabel = this->m_labels.begin();
		iter != this->m_timers.end() && iterLabel != this->m_labels.end();
		i++, iter++, iterLabel++)
	{
		time = iter->GetTimeMS(this->m_frequency);

		minTime = this->m_timerMins.at(i);
		maxTime = this->m_timerMaxs.at(i);
		this->m_timerMins.at(i) = (minTime < time) ? minTime : time;
		this->m_timerMaxs.at(i) = (maxTime > time) ? maxTime : time;

		LARGE_INTEGER elapsedTime;
		elapsedTime.QuadPart = this->m_programEnd.QuadPart - this->m_programStart.QuadPart;
		elapsedTime.QuadPart *= 1000000;
		elapsedTime.QuadPart /= this->m_frequency.QuadPart;

		std::cout << std::fixed << std::setprecision(1) << iterLabel->c_str() << ": [" << this->m_timerMins.at(i) << "] "
			<< time << " [" << this->m_timerMaxs.at(i) << "] us, " 
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

	this->m_timers.clear();
	this->m_labels.clear();
	this->m_timerToEnd = 0;

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
		SetConsoleCursorPosition(console, FPSLocation);
		std::cout << "FPS: " << avgFPS << " (" << std::to_string(this->m_frameTimes[this->m_currFrameTimesPtr]) << ")";
		GetConsoleScreenBufferInfo(console, &screen);
		FillConsoleOutputCharacterA(
			console, ' ', 5, screen.dwCursorPosition, &written
		);
		this->m_currFrameTimesPtr++;
	}

	return 0;
}

void DebugHandler::Shutdown()
{
	if (m_instance != nullptr) delete this->m_instance;
}
