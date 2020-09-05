// Adapted from https://gist.github.com/mcleary/b0bf4fa88830ff7c882d
#pragma once
#include <chrono>
#include <ctime>
class Timer
{
public:
	void Start();
	double Tick();
	void Stop();
private:
	std::chrono::time_point<std::chrono::system_clock> m_StartTime;
	std::chrono::time_point<std::chrono::system_clock> m_EndTime;
	std::chrono::time_point<std::chrono::system_clock> m_lastTick;
	bool m_bRunning = false;

};

