#pragma once
#include <cstdint>
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds MS;
typedef std::chrono::duration<float> fsec;

class Timer
{
public:
  Timer();

  void start();
  float getTimeRunning();

private:
  Clock::time_point currentTime();

  Clock::time_point m_startTime;
};
