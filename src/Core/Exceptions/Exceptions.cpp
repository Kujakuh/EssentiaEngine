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

        //std::exit(EXIT_FAILURE);  // Termina el programa con un código de error.
        std::terminate();
    }

    TerminateHandler::TerminateHandler()
    {
        std::set_terminate(customTerminate);
    }

    TerminateHandler TerminateHandler::instance;
}
