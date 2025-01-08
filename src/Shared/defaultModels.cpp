#include <Shared/defaultModels.hpp>

namespace Essentia
{
    Mesh cube;
    Mesh sphere;

    void initDefaultModels()
    {
        // SPHERE DATA ------------------------------------------------
        std::vector<Vertex> sphereVertices;
        std::vector<unsigned int> sphereIndices;

        float radius = 1.0f;                // Radio
        unsigned int sectorCount = 36;      // Divisiones horizontales
        unsigned int stackCount = 18;       // Divisiones verticales
        // SPHERE DATA------------------------------------------------

        cube = Mesh(
            nullptr,
            Essentia::cubeVertices,
            Essentia::cubeIndices,
            { Material() }
        );

        generateSphereData(sphereVertices, sphereIndices, radius, sectorCount, stackCount);

        sphere = Mesh(
            nullptr,
            sphereVertices,
            sphereIndices,
            { Material() }
        );
    }

    void generateSphereData(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, float radius, unsigned int sectorCount, unsigned int stackCount)
    {
        float x, y, z, xy;                              // Posición del vértice
        float nx, ny, nz, lengthInv = 1.0f / radius;    // Normales
        float s, t;                                     // Coordenadas de textura

        float sectorStep = 2.0f * glm::pi<float>() / sectorCount;
        float stackStep = glm::pi<float>() / stackCount;
        float sectorAngle, stackAngle;

        // Generar vértices
        for (unsigned int i = 0; i <= stackCount; ++i)
        {
            stackAngle = glm::pi<float>() / 2 - i * stackStep; // De pi/2 a -pi/2
            xy = radius * glm::cos(stackAngle);               // Radio * cos(stackAngle)
            z = radius * glm::sin(stackAngle);                // Radio * sin(stackAngle)

            // Sector
            for (unsigned int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;

                // Posición del vértice
                x = xy * glm::cos(sectorAngle);
                y = xy * glm::sin(sectorAngle);

                // Normales
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;

                // Coordenadas de textura
                s = (float)j / sectorCount;
                t = (float)i / stackCount;

                vertices.push_back(Vertex(glm::vec3(x, y, z), glm::vec3(nx, ny, nz), glm::vec2(s, t)));
            }
        }

        // Generar índices
        unsigned int k1, k2;
        for (unsigned int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1); // Inicio de la fila actual
            k2 = k1 + sectorCount + 1; // Inicio de la próxima fila

            for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                if (i != 0) // No añadir el primer triángulo de la pila
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != (stackCount - 1)) // No añadir el último triángulo de la pila
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
    }
}
