#include <Framework/Time/Time.hpp>

namespace Essentia
{
    std::chrono::time_point<std::chrono::high_resolution_clock> Time::startTime = std::chrono::high_resolution_clock::now();
    std::chrono::time_point<std::chrono::high_resolution_clock> Time::lastFrame = Time::startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> Time::lastFpsUpdate = Time::startTime;
    int Time::_fps = 0;
    int Time::_frameCount = 0;
    float Time::_deltaTime = 0.0f;
    std::vector<std::shared_ptr<Timer>> Time::timers;

    double Time::now() 
    {
        return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startTime).count();
    }

    void Time::update()
    {
        auto now = std::chrono::high_resolution_clock::now();
        _deltaTime = std::chrono::duration<float>(now - lastFrame).count();
        lastFrame = now;

        _frameCount++;
        if (std::chrono::duration<float>(now - lastFpsUpdate).count() >= 1.0f)
        {
            _fps = _frameCount;
            _frameCount = 0;
            lastFpsUpdate = now;
        }

        updateTimers();
    }

    float Time::deltaTime() { return _deltaTime; }

    int Time::fps() { return _fps; }

    void Time::addTimer(std::shared_ptr<Timer> timer)
    {
        auto it = std::find(timers.begin(), timers.end(), timer);
        if (it == timers.end()) {
            timer->reset();
            timers.push_back(timer);
        }
    }

    void Time::updateTimers()
    {
        for (auto& timer : timers) 
            timer->update(_deltaTime);

        timers.erase(std::remove_if(timers.begin(), timers.end(), [](const std::shared_ptr<Timer>& timer) 
        {
            return timer->isDone();
        }), timers.end());
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
