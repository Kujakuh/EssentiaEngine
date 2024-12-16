#include <Shared/defaultModels.hpp>

namespace Essentia
{
    Mesh cube;

    void initDefaultModels()
    {
        cube = Mesh(
            nullptr,
            Essentia::cubeVertices,
            Essentia::cubeIndices,
            { Material() }
        );

    }
}
