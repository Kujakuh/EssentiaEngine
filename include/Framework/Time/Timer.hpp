#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <iostream>

namespace Essentia
{
    class Timer 
    {
        public:
            float duration;

            Timer(float duration, std::function<void()> callback = []() {});

            void update(float deltaTime);

            bool isDone() const;
            float getElapsedTime() const;

            void stop();
            void pause();
            void resume();
            void reset();

        private:
            std::function<void()> callback;
            float elapsedTime;
            bool done; 
            bool paused;
    };
}

#endif // ESSENTIA_TIMER_HPP