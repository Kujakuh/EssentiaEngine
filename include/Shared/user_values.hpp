#ifndef USER_VAL_H
#define USER_VAL_H

#include <flat_hash_map>
#include <Shared/enums.hpp>
#include <glad/glad.h>

namespace Essentia
{
    extern ska::flat_hash_map<FILTERS, GLenum> defaultFilters;
    void init();
}

#endif // !USER_VAL_H