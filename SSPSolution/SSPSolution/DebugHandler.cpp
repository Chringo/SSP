#include "DebugHandler.h"

DebugHandler* DebugHandler::m_instance = nullptr;

DebugHandler::DebugHandler()
{
	QueryPerformanceFrequency(&this->m_frequency);
	this->m_timerToEnd = 0;
	this->m_displayFPS = true;
	this->ClearConsole();
}

void DebugHandler::ClearConsole()
{
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', 400, topLeft, &written
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
		
		std::cout << iterLabel->c_str() << ": [" << this->m_timerMins.at(i) << "] "
			<< time << " [" << this->m_timerMaxs.at(i) << "] us";
		GetConsoleScreenBufferInfo(console, &screen);
		FillConsoleOutputCharacterA(
			console, ' ', 10, screen.dwCursorPosition, &written
		);
		std::cout << std::endl;
	}

	this->m_timers.clear();
	this->m_labels.clear();
	this->m_timerToEnd = 0;

	if (this->m_displayFPS)
	{
		SetConsoleCursorPosition(console, FPSLocation);
		std::cout << "FPS: " << std::to_string((int)(1000000 / dTime));
		GetConsoleScreenBufferInfo(console, &screen);
		FillConsoleOutputCharacterA(
			console, ' ', 10, screen.dwCursorPosition, &written
		);
	}

	return 0;
}

void DebugHandler::Shutdown()
{
	if (m_instance != nullptr) delete this->m_instance;
}
