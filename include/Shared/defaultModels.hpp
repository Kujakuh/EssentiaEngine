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

    void initDefaultModels();
}

#endif // !DEFAULT_MODELS_H
