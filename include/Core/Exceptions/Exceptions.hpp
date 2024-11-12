#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include <Core/Exceptions/UnreachableEntityException.hpp>

namespace Essentia
{
    // Declaraci�n de la funci�n customTerminate
    void customTerminate();

    class TerminateHandler
    {
    public:
        // Constructor est�tico que configura el manejador de terminaci�n
        TerminateHandler();

        // Instancia est�tica que asegura que el manejador se registre al inicio
        static TerminateHandler instance;
    };
}

#endif // !EXCEPTIONS_H
