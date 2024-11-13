#ifndef UNR_ENT_H
#define UNR_ENT_H

#include <stdexcept>
#include <string>

namespace Essentia
{
    class UnreachableEntityException : public std::runtime_error
    {
        public:
            UnreachableEntityException(const std::string& message)
                : std::runtime_error("Unreachable entity. Cannot perform requested operation: " + message) {}
    };
}

#endif //!UNR_ENT_H