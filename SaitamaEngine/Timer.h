#pragma once
#include <chrono>

class Timer
{
public:
    Timer();
    double GetMillisecondsElapsed();
    void Restart();
    bool Stop();
    bool Start();
private:
    bool m_isRunning = false;
    std::chrono::time_point<std::chrono::steady_clock> m_start;
    std::chrono::time_point<std::chrono::steady_clock> m_stop;

};