#ifndef DEFAULT_MODELS_H
#define DEFAULT_MODELS_H

#include <vector>
#include <flat_hash_map>
#include <glad/glad.h>

#include <Shared/user_values.hpp>
#include <Core/Graphics/ShaderLab.hpp>
#include <Core/Graphics/Shader.hpp>
#include <Core/Graphics/Material.hpp>
#include <Core/Graphics/Mesh.hpp>

namespace Essentia
{
    extern Mesh cube;
    extern Mesh sphere;

    void initDefaultModels();
    void generateSphereData(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, float radius, unsigned int sectorCount, unsigned int stackCount);
}

#endif // !DEFAULT_MODELS_H
