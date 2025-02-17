#ifndef INIT_EVENT_H
#define INIT_EVENT_H

#include <Core/EventSystem/IEvent.hpp>

namespace Essentia
{
    class InitEvent : public IEvent
    {
        public:
            std::string getInfo() const override
            {
                return info;
            }

            std::string getType() const override
            {
                return "InitEvent";
            }

        private:
            std::string info = "This is a Init testing event";
        };
}

#endif //!INIT_EVENT_H
