#include <Core/Exceptions/Exceptions.hpp>

#include <stdexcept>
#include <iostream>
#include <cstdlib>

namespace Essentia
{
    void customTerminate()
    {
        try
        {
            throw;
        }
        catch (const std::runtime_error& e)
        {
            std::cerr << "UNHANDLED EXCEPTION: " << e.what() << std::endl;
        }

        //std::exit(EXIT_FAILURE);  // Termina el programa con un c�digo de error.
        std::terminate();
    }

    // Constructor est�tico que configura el manejador de terminaci�n
    TerminateHandler::TerminateHandler()
    {
        std::set_terminate(customTerminate);
    }

    // Inicializaci�n est�tica de la instancia
    TerminateHandler TerminateHandler::instance;
}