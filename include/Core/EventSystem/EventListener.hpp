#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include <Shared/enums.hpp>

namespace Essentia
{
    class EventListener
    {
        public:
            virtual void onSysEvent(INTERNAL_EVENT event) {}
            virtual void onEvent(int event) {}

            virtual ~EventListener() = default;
    };
}

#endif //!EVENT_LISTENER_H