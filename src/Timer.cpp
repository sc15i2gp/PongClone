#include "Timer.hpp"

Timer::Timer()
{
}

void Timer::start()
{
	m_startTime = currentTime();
}

float Timer::getTimeRunning()
{
	fsec timeRunning = currentTime() - m_startTime;
	return timeRunning.count() * 1000;
}

Clock::time_point Timer::currentTime()
{
	return Clock::now();
}
