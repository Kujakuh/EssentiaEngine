#include <Framework/Time/Time.hpp>

namespace Essentia
{
    std::chrono::time_point<std::chrono::high_resolution_clock> Time::startTime = std::chrono::high_resolution_clock::now();
    std::chrono::time_point<std::chrono::high_resolution_clock> Time::lastFrame = Time::startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> Time::lastFpsUpdate = Time::startTime;
    int Time::fps = 0;
    int Time::frameCount = 0;
    float Time::deltaTime = 0.0f;
    std::vector<std::shared_ptr<Timer>> Time::timers;

    double Time::now() 
    {
        return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startTime).count();
    }

    void Time::update()
    {
        auto now = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float>(now - lastFrame).count();
        lastFrame = now;

        frameCount++;
        if (std::chrono::duration<float>(now - lastFpsUpdate).count() >= 1.0f)
        {
            fps = frameCount;
            frameCount = 0;
            lastFpsUpdate = now;
        }

        updateTimers();
    }

    float Time::getDeltaTime() { return deltaTime; }

    int Time::getFps() { return fps; }

    void Time::addTimer(std::shared_ptr<Timer> timer) { timers.push_back(timer); }

    void Time::updateTimers()
    {
        for (auto& timer : timers) 
            timer->update(deltaTime);
    }

    void Time::stopAllTimers()
    {
        for (auto& timer : timers)
            timer->stop();
    }

    void Time::pauseAllTimers()
    {
        for (auto& timer : timers)
            timer->pause();
    }

    void Time::resumeAllTimers() 
    {
        for (auto& timer : timers)
            timer->resume();
    }

}
