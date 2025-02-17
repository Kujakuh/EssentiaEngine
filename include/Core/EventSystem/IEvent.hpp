#ifndef IEVENT_H
#define IEVENT_H

#include <string>

namespace Essentia
{
    class IEvent
    {
        public:
            virtual ~IEvent() = default;

            virtual std::string getInfo() const = 0;
            virtual std::string getType() const = 0;

            virtual void debugLog() const
            {
                printf("[EVENT] Type: %s, Info: %s\n", getType().c_str(), getInfo().c_str());
            }
    };
}

#endif //!IEVENT_H