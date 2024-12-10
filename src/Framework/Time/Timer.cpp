#include <Framework/Time/Timer.hpp>

namespace Essentia
{
    Timer::Timer(std::function<void()> callback, float duration)
        : callback(callback), duration(duration), elapsedTime(0.0f), done(false), paused(false) {}

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
    void Timer::stop() { done = true; }
    void Timer::pause() { paused = true; }
    void Timer::resume() { if (!done) paused = false; }
}