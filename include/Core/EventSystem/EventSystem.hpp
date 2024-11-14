#ifndef EVENT_SYS_H
#define EVENT_SYS_H

#include <Core/EventSystem/EventListener.hpp>

#include <type_traits>
#include <unordered_set>
#include <stdexcept>

namespace Essentia
{
    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    class EventSystem
    {
        private:
            EventSystem() = default;
            std::unordered_set<EventListener*> listeners;

        public:
            static void addListener(EventListener* listener) { instance().listeners.insert(listener); }
            static void removeListener(EventListener* listener) { instance().listeners.erase(listener); }

            static void emit(T eventType)
            {
                if constexpr (std::is_same_v<T, INTERNAL_EVENT>)
                {
                    if (!instance().allowInternalEvents)
                        throw std::runtime_error("ERROR: Event emission for system events is disabled in the current context.");
                }

                for (auto* listener : instance().listeners)
                    listener->onEvent(eventType);
            }

        private:
            static EventSystem& instance()
            {
                static EventSystem instance;
                return instance;
            }

            static void emitInternalEvent(T eventType)
            {
                if constexpr (!std::is_same_v<T, INTERNAL_EVENT>) 
                    throw std::runtime_error("ERROR: Only INTERNAL_EVENT types can be emitted via this method.");
                
                for (auto* listener : instance().listeners)
                    listener->onSysEvent(eventType);
            }

        friend class Scene;
        friend class Entity;
        friend class EntityManager;
        friend class SceneManager;
        friend class SystemDispatcher;
    };
}

#endif //!EVENT_SYS_H