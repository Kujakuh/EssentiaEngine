#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <iostream>

namespace Essentia
{
    class Timer 
    {
        public:
            Timer(std::function<void()> callback, float duration);

            void update(float deltaTime);

            bool isDone() const;

            void stop();
            void pause();
            void resume();

        private:
            std::function<void()> callback;
            float duration;
            float elapsedTime;
            bool done; 
            bool paused;
    };
}

#endif // ESSENTIA_TIMER_HPP