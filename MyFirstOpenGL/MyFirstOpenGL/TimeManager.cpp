#include "TimeManager.h"

#include <GLFW/glfw3.h>

TimeManager::TimeManager()
    :
    deltaTime(0.0f),
    lastFrame(0.0f),
    currentTime(0.0f),
    timeScale(1.0f),
    paused(false)
{
    lastFrame = glfwGetTime();
}

void TimeManager::Update()
{
    float now = glfwGetTime();

    deltaTime = now - lastFrame;

    lastFrame = now;

    if (!paused)
    {
        currentTime += deltaTime * timeScale;
    }
}

float TimeManager::GetDeltaTime() const
{
    return deltaTime;
}

float TimeManager::GetTime() const
{
    return currentTime;
}

void TimeManager::SetPaused(bool state)
{
    paused = state;
}

bool TimeManager::IsPaused() const
{
    return paused;
}

void TimeManager::SetTimeScale(float scale)
{
    timeScale = scale;
}

float TimeManager::GetTimeScale() const
{
    return timeScale;
}