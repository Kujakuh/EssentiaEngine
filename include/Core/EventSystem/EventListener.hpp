#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include <Core/enums.hpp>

namespace Essentia
{
    class EventListener
    {
        public:
            virtual void onEvent(INTERNAL_EVENT event) {}

            template<typename T>
            void onEvent(T event) {}

            virtual ~EventListener() = default;
    };
}

#endif //!EVENT_LISTENER_H