#ifndef EVENT_SYS_H
#define EVENT_SYS_H

#include <Core/EventSystem/EventListener.hpp>

#include <type_traits>
#include <unordered_set>

template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
class EventSystem
{
    public:
        static EventSystem& getInstance()
        {
            static EventSystem instance;
            return instance;
        }

        void addListener(EventListener* listener) { listeners.insert(listener); }

        void removeListener(EventListener* listener) { listeners.erase(listener); }

        void emit(T eventType)
        {
            for (auto* listener : listeners)
                listener->onEvent(eventType);
        }

    private:
        EventSystem() = default;
        std::unordered_set<EventListener*> listeners;
};

#endif //!EVENT_SYS_H