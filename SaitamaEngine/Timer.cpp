#include "pch.h"
#include "Timer.h"

Timer::Timer()
{
    m_start = std::chrono::high_resolution_clock::now();
    m_stop = std::chrono::high_resolution_clock::now();
}

double Timer::GetMillisecondsElapsed()
{
    if (m_isRunning)
    {
        auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - m_start);
        return elapsed.count();
    }
    else
    {
        auto elapsed = std::chrono::duration<double, std::milli>(m_stop - m_start);
        return elapsed.count();
    }
}

void Timer::Restart()
{
    m_isRunning = true;
    m_start = std::chrono::high_resolution_clock::now();
}

bool Timer::Stop()
{
    if (!m_isRunning)
    {
        return false;
    }
    else
    {
        m_stop = std::chrono::high_resolution_clock::now();
        m_isRunning = false;
        return true;
    }

}

bool Timer::Start()
{
    if (m_isRunning)
    {
        return false;
    }
    else
    {
        m_start = std::chrono::high_resolution_clock::now();
        m_isRunning = true;
        return true;
    }
}
