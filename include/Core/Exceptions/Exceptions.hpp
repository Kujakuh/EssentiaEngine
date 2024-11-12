#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include <Core/Exceptions/UnreachableEntityException.hpp>

namespace Essentia
{
    // Declaración de la función customTerminate
    void customTerminate();

    class TerminateHandler
    {
    public:
        // Constructor estático que configura el manejador de terminación
        TerminateHandler();

        // Instancia estática que asegura que el manejador se registre al inicio
        static TerminateHandler instance;
    };
}

#endif // !EXCEPTIONS_H
