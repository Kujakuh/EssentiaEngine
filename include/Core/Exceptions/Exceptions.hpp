#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include <Core/Exceptions/UnreachableEntityException.hpp>

namespace Essentia
{
    void customTerminate();

    class TerminateHandler
    {
        public:
            TerminateHandler();

            static TerminateHandler instance;
    };
}

#endif // !EXCEPTIONS_H
