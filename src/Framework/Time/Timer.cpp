#include <Framework/Time/Timer.hpp>

namespace Essentia
{
    Timer::Timer(float duration, std::function<void()> callback)
        : callback(callback), duration(duration), elapsedTime(0.0f), done(true), paused(false) {}

    void Timer::update(float deltaTime)
    {
        if (done || paused) return;

        elapsedTime += deltaTime;

        if (elapsedTime >= duration)
        {
            callback();
            done = true;
        }
    }

    bool Timer::isDone() const { return done; }
    float Timer::getElapsedTime() const { return elapsedTime; }

    void Timer::stop() { done = true; }
    void Timer::pause() { paused = true; }
    void Timer::resume() { if (!done) paused = false; }
    void Timer::reset() 
    {
        elapsedTime = 0.0f;
        done = false;
        paused = false;
    }
}