#include "Timer.h"

void Timer::Start()
{
	m_StartTime = std::chrono::system_clock::now();
	m_bRunning = true;
}

double Timer::Tick()
{

	if (m_bRunning)
	{
		//endTime = std::chrono::system_clock::now();
		std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
		double result = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastTick).count();
		m_lastTick = now;
		return result / 1000;
	}
	else
	{
		return -1;
	}
	//else
	//{
	//	endTime = m_EndTime;
	//}

	//return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
}

void Timer::Stop()
{
	m_EndTime = std::chrono::system_clock::now();
	m_bRunning = false;
}
