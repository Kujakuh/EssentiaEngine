#ifndef USER_VAL_H
#define USER_VAL_H

#include <vector>

#include <flat_hash_map>
#include <glad/glad.h>

#include <Shared/enums.hpp>
#include <Core/Graphics/Vertex.hpp>

namespace Essentia
{
    extern ska::flat_hash_map<FILTERS, GLenum> defaultFilters;
    extern std::vector<Vertex> cubeVertices;
    extern std::vector<GLuint> cubeIndices;

    void init();
}

#endif // !USER_VAL_H