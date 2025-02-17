#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include <Core/EventSystem/IEvent.hpp> 

namespace Essentia
{
    class EventListener
    {
        public:
            virtual void onEvent(const IEvent& event) = 0;
    };
}

#endif //!EVENT_LISTENER_H