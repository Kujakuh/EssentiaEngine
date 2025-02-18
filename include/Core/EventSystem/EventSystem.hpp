#ifndef EVENT_SYS_H
#define EVENT_SYS_H

#include <Core/EventSystem/EventListener.hpp>
#include <Core/EventSystem/IEvent.hpp>

#include <unordered_set>

namespace Essentia
{
    class EventSystem
    {
        private:
            EventSystem() = default;
            std::unordered_set<EventListener*> listeners;

        public:
            static EventSystem& instance()
            {
                static EventSystem instance;
                return instance;
            }

            static void addListener(EventListener* listener)
            {
                instance().listeners.insert(listener);
            }

            static void removeListener(EventListener* listener)
            {
                instance().listeners.erase(listener);
            }

            static void emit(const IEvent& event)
            {
                instance().emitInternal(event);
            }

        private:
            void emitInternal(const IEvent& event)
            {
                for (auto* listener : listeners)
                {
                    listener->onEvent(event);
                }
            }
    };
}

#endif //!EVENT_SYS_H
