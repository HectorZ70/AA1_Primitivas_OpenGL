#pragma once

class TimeManager
{
private:

    float deltaTime;
    float lastFrame;
    float currentTime;

    float timeScale;

    bool paused;

public:

    TimeManager();

    void Update();

    float GetDeltaTime() const;

    float GetTime() const;

    void SetPaused(bool state);

    bool IsPaused() const;

    void SetTimeScale(float scale);

    float GetTimeScale() const;
};

