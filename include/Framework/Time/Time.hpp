#ifndef TIME_H
#define TIME_H

#include <chrono>
#include <vector>
#include <memory>
#include <functional>

#include <Framework/Time/Timer.hpp>

namespace Essentia 
{
    class Time
    {
        public:
            static void update();

            static double now();
            static float deltaTime();
            static int fps();

            static void addTimer(std::shared_ptr<Timer> timer);

            static void updateTimers();
            static void stopAllTimers();
            static void pauseAllTimers();
            static void resumeAllTimers();

        private:
            static std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
            static std::chrono::time_point<std::chrono::high_resolution_clock> lastFrame;
            static std::chrono::time_point<std::chrono::high_resolution_clock> lastFpsUpdate;
            static int _fps;
            static int _frameCount;
            static float _deltaTime;

            static std::vector<std::shared_ptr<Timer>> timers;
    };
}

#endif // !TIME_H